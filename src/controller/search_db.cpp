// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "controller/search_db.h"
#include "base/utils.h"
#include "base/ddlog.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QUuid>

namespace {

const char kSearchDropTable[] = "DROP TABLE IF EXISTS search";

//--------------- search表 sql语句 -------------------
const char kSearchTableSchema[] =
    "CREATE TABLE IF NOT EXISTS search "
    "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "appName TEXT,"
    "lang TEXT,"
    "anchor TEXT,"
    "anchorInitial TEXT,"
    "anchorSpell TEXT,"
    "anchorId TEXT,"
    "content TEXT)";

const char kSearchIndexSchema[] =
    "CREATE INDEX IF NOT EXISTS search_idx "
    "ON search (id, appName, lang)";

const char kSearchDeleteEntryByApp[] =
    "DELETE FROM search "
    "WHERE appName = ? AND lang = ?";

const char kSearchInsertEntry[] =
    "INSERT INTO search "
    "(appName, lang, anchor, anchorInitial, anchorSpell, anchorId, content) "
    "VALUES (?, ?, ?, ?, ?, ?, ?)";

// const char kSearchSelectAll[] = "SELECT * FROM search";

const char kSearchSelectAnchor[] =
    "SELECT t1.appName, t2.anchor, t1.anchor, t1.anchorId FROM search t1 "
    "LEFT JOIN (SELECT anchor,appName FROM search where anchorId='h0' and lang=':lang') t2 ON t1.appName=t2.appName "
    "WHERE t1.appName=t2.appName "
    "AND t1.lang = ':lang' "
    "AND (t1.anchor LIKE '%/:anchor%' escape '/' "
    "OR t1.anchorSpell LIKE '%/:anchor%' escape '/'"
    "OR t1.anchorInitial LIKE '%/:anchor%' escape '/') --case insensitive";

//将包含关键字的搜索结果顺序输出
const char kSearchSelectContent[] =
    //    "SELECT appName, anchor, anchorId, content "
    //    "FROM search "
    //    "WHERE lang = ':lang' AND "
    //    "content LIKE '%:content%' --case insensitive";
    "select appName, anchor, anchorId, content from search where lang = ':lang' and anchor like '%/:content%' escape '/' "
    "union all "
    "select appName, anchor, anchorId, content from search where lang = ':lang' and content like '%/:content%' escape '/' and anchor not like '%/:content%' escape '/'"
    "order by appName";

//--------------  filetime表sql语句  -----------------
//建表
const char kfileTimeTable[] =
    "CREATE TABLE IF NOT EXISTS filetime "
    "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "mdPath TEXT,"
    "time TEXT)";
//删除
const char kfileTimeDeleteEntryByApp[] =
    "DELETE FROM filetime "
    "WHERE mdPath = ?";
//插入
const char kfileTimeInsertEntry[] =
    "INSERT INTO filetime "
    "(mdPath, time) "
    "VALUES (?, ?)";
//查找所有appName lang
const char kfileTimeSelectAll[] =
    "SELECT mdPath, time "
    "FROM filetime";
//根据 appName lang 查询time
const char kfileTimeSelectByTime[] =
    "SELECT time "
    "FROM filetime "
    "WHERE appName = ':appName' AND lang = ':lang'";
//根据 appName lang 更新time
const char kfileTimeUpdate[] =
    "UPDATE filetime "
    "SET time = ':time' "
    "WHERE appName = ':appName' AND lang = ':lang'";

const int kResultLimitation = INT_MAX;

} // namespace

struct SearchDbPrivate {
    QSqlDatabase db;
};

struct searchStrct {
    QString appName;
    QStringList anchors;
    QStringList anchorIds;
    QStringList contents;
};

SearchDb::SearchDb(QObject *parent)
    : QObject(parent)
    , p_(new SearchDbPrivate())
{
    qCDebug(app) << "Creating SearchDb instance";
    qRegisterMetaType<SearchAnchorResult>("SearchAnchorResult");
    qRegisterMetaType<SearchAnchorResultList>("SearchAnchorResultList");
    qRegisterMetaType<SearchContentResult>("SearchContentResult");
    qRegisterMetaType<SearchContentResultList>("SearchContentResultList");
    qCDebug(app) << "Meta types registered";

    initConnections();
    getAllApp();
    qCDebug(app) << "SearchDb initialized";
}

SearchDb::~SearchDb()
{
    qCDebug(app) << "Destroying SearchDb instance";
    if (p_ != nullptr) {
        qCDebug(app) << "Closing database connection";
        if (p_->db.isOpen()) {
            qCDebug(app) << "Closing database connection 1";
            p_->db.close();
        }
        delete p_;
        p_ = nullptr;
    }
    qCDebug(app) << "SearchDb destroyed";
}

/**
 * @brief SearchDb::initConnections
 * 绑定信号槽
 */

void SearchDb::initConnections()
{
    qCDebug(app) << "Initializing search database connections";
    connect(this, &SearchDb::initDbAsync, this, &SearchDb::initDb);
    connect(this, &SearchDb::searchAnchor, this, &SearchDb::handleSearchAnchor);
    connect(this, &SearchDb::searchContent, this, &SearchDb::handleSearchContent);
    connect(this, &SearchDb::updateModule, this, &SearchDb::getAllApp);
    qCDebug(app) << "Search database connections initialized";
}

/**
 * @brief SearchDb::initDb
 * @param db_path 数据库文件路径
 * 数据库操作初始化
 */
void SearchDb::initDb()
{
    qCDebug(app) << "Initializing database";
    //获取本地配置文件目录
    QString dbdir = Utils::mkMutiDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append("/.local/share/deepin/deepin-manual"));
    qCDebug(app) << dbdir;
    QDir dir(dbdir);
    if (!dir.exists()) {
        qCCritical(app) << "db path not exist!" << dbdir;
    }
    QString databasePath = dbdir.append("/search.db");
    qCDebug(app) << "Database file path:" << databasePath;

    //创建数据库
    p_->db = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString(QUuid::WithoutBraces));
    p_->db.setDatabaseName(databasePath);
    if (!p_->db.open()) {
        qCCritical(app) << "Failed to open search db:" << databasePath << p_->db.lastError().text()
                    << p_->db.lastError().nativeErrorCode() << p_->db.lastError().type()
                    << p_->db.lastError().databaseText() << p_->db.lastError().driverText()
                    << p_->db.lastError().isValid();
    }
}

/**
 * @brief SearchDb::initSearchTable
 * 数据库search表初始化操作
 */
void SearchDb::initSearchTable()
{
    Q_ASSERT(p_->db.isOpen());
    qCDebug(app) << "Initializing search table";
    
    QSqlQuery query(p_->db);
    qCDebug(app) << "Executing search table schema:" << kSearchTableSchema;

    //创建相关表
    if (!query.exec(kSearchTableSchema)) {
        qCCritical(app) << "Failed to initialize search table:" << query.lastError().text();
        return;
    }
    
    qCDebug(app) << "Creating search index with schema:" << kSearchIndexSchema;
    
    if (!query.exec(kSearchIndexSchema)) {
        qCCritical(app) << "Failed to create search index:"
                       << "error:" << query.lastError().text()
                       << "query:" << query.lastQuery();
        return;
    }
    
    qCDebug(app) << "Search table initialized successfully";
}

/**
 * @brief SearchDb::addSearchEntry
 * @param system            文件版本，如：professional专业版 server服务器版
 * @param app_name          应用名称
 * @param lang              语言 如：zh_CN中文
 * @param anchors           标题 md文件中以“#”开头
 * @param anchorInitialList 首字母字段
 * @param anchorSpellList   全拼字段
 * @param anchorIdList      标题Id 如：h1, h2 ...
 * @param contents          内容
 * 解析md文件之后，生成数据，插入到数据库
 */
void SearchDb::addSearchEntry(const QString &app_name, const QString &lang,
                              const QStringList &anchors, const QStringList &anchorInitialList,
                              const QStringList &anchorSpellList, const QStringList &anchorIdList,
                              const QStringList &contents, const QString &mdPath)
{
    Q_ASSERT(p_->db.isOpen());
    Q_ASSERT(anchors.length() == contents.length());
    qCDebug(app) << "Adding search entry:"
               << "app:" << app_name
               << "lang:" << lang
               << "anchors:" << anchors.size()
               << "contents:" << contents.size();
    QStringList newContents = contents;

    if (mdPath.isEmpty()) {
        qCDebug(app) << "mdPath is empty";
        QString strManualPath;
        if (app_name == "dde") {
            strManualPath = "/system";
            qCDebug(app) << "app_name is dde";
        } else {
            strManualPath = "/application";
            qCDebug(app) << "app_name is application";
        }

        //图片路径替换为绝对路径
        for (int i = 0; i < contents.size(); i++) {
            QString content = contents.at(i);
            content = content.replace("icon/", DMAN_INSTALL_DB_PATH + strManualPath
                                      + "/" + app_name + "/" + lang + "/icon/");
            newContents.replace(i, content);
            qCDebug(app) << "newContent replace icon:" << i;
        }
    } else {
        qCDebug(app) << "mdPath is not empty";
        QStringList list = mdPath.split("/");
        list.removeLast();
        QString strTemp = list.join("/");
        for (int i = 0; i < contents.size(); i++) {
            QString content = contents.at(i);
            //正则替换所有的svg路径
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
            QRegExp exp("src=\"([^>]*svg)\"");
            exp.setMinimal(true);
            QRegExp expPng("src=\"..\/common([^>]*png)\"");
            expPng.setMinimal(true);
#else
            QRegularExpression exp("src=\"([^>]*svg)\"");
            QRegularExpression expPng("src=\"..\/common([^>]*png)\"");
#endif

            content = content.replace(exp, QString("src=\"%1/%2\"").arg(strTemp).arg("\\1"));
            if (content.contains("common")/* && !content.contains("fig")*/)
                content = content.replace(expPng, QString("src=\"%1/../common%2\"").arg(strTemp).arg("\\1"));
            newContents.replace(i, content);
            qCDebug(app) << "newContent replace svg:" << i;
        }
    }

    if (anchors.length() != newContents.length() || anchors.length() != anchorIdList.length()) {
        qCCritical(app) << "anchor list and contents mismatch:" << anchors.length()
                    << newContents.length();
        return;
    }

    QSqlQuery query(p_->db);
    bool preok = query.prepare(kSearchDeleteEntryByApp);
    if (!preok) {
        qCCritical(app) << "Failed to prepare skSearchDeleteEntryByApp:" << query.lastError().text();
        return;
    }
    query.bindValue(0, app_name);
    query.bindValue(1, lang);
    if (!query.exec()) {
        qCCritical(app) << "Failed to delete search entry:" << query.lastError().text();
        return;
    }

    qCDebug(app) << "Starting database transaction";
    if (!p_->db.transaction()) {
        qCWarning(app) << "Failed to start database transaction:"
                      << "error:" << p_->db.lastError().text();
        return;
    }
    qCDebug(app) << "Transaction started successfully";
    preok = query.prepare(kSearchInsertEntry);
    if (!preok) {
        qCCritical(app) << "Failed to prepare kSearchInsertEntry:" << query.lastError().text();
        return;
    }
    QStringList app_names;
    QStringList lang_list;
    for (int i = 0; i < anchors.length(); ++i) {
        app_names.append(app_name);
        lang_list.append(lang);
    }
    int icout = app_names.size();
    if (icout == lang_list.size() && icout == anchors.size() && icout == anchorInitialList.size() && icout == anchorSpellList.size() && icout == anchorIdList.size() && icout == newContents.size()) {
        query.bindValue(0, app_names);
        query.bindValue(1, lang_list);
        query.bindValue(2, anchors);
        query.bindValue(3, anchorInitialList);
        query.bindValue(4, anchorSpellList);
        query.bindValue(5, anchorIdList);
        query.bindValue(6, newContents);
        bool ok = query.execBatch();

        if (!ok) {
            p_->db.rollback();
            qCCritical(app) << "Failed to insert search entry:"
                          << "error:" << query.lastError().text()
                          << "query:" << query.lastQuery();
        } else {
            p_->db.commit();
            qCDebug(app) << "Search entry added successfully";
        }
    } else {
        qCWarning(app) << "Field size mismatch, execBatch may crash:"
                     << "app:" << app_name
                     << "anchors:" << anchors.size()
                     << "contents:" << contents.size()
                     << "anchorIds:" << anchorIdList.size();
    }
    qCDebug(app) << "Ending database transaction";
}

/**
 * @brief SearchDb::deleteSearchInfo
 * @param appName
 * @param lang
 * 删除数据库操作
 */
void SearchDb::deleteSearchInfo(const QStringList &appName, const QStringList &lang)
{
    Q_ASSERT(p_->db.isOpen());
    qCDebug(app) << "Deleting search entry";
    QSqlQuery query(p_->db);
    bool preok = query.prepare(kSearchDeleteEntryByApp);
    if (!preok) {
        qCCritical(app) << "Failed to prepare kSearchDeleteEntryByApp:" << query.lastError().text();
        return;
    }
    query.bindValue(0, appName);
    query.bindValue(1, lang);
    if (!query.execBatch()) {
        qCCritical(app) << "Failed to delete search entry:" << query.lastError().text();
        return;
    }
    qCDebug(app) << "Search entry deleted successfully";
}

/**
 * @brief SearchDb::initTimeTable 初始化创建fileTime表
 */
void SearchDb::initTimeTable()
{
    qCDebug(app) << "Initializing filetime table";
    QSqlQuery query(p_->db);
    qCDebug(app) << "Creating filetime table with schema:" << kfileTimeTable;
    
    if (!query.exec(kfileTimeTable)) {
        qCCritical(app) << "Failed to create filetime table:"
                       << "error:" << query.lastError().text()
                       << "query:" << query.lastQuery();
        return;
    }
    qCDebug(app) << "Filetime table initialized successfully";
}

/**
 * @brief SearchDb::handleSearchAnchor
 * @param keyword 搜索关键字
 * 根据keyword 在数据库中匹配，匹配结果通过searchAnchorResult()信号发出
 */
void SearchDb::handleSearchAnchor(const QString &keyword)
{
    qCDebug(app) << "Searching anchor for keyword:" << keyword;
    Q_ASSERT(p_->db.isOpen());

    SearchAnchorResultList result;

    QSqlQuery query(p_->db);
    QString lang = QLocale().name();
    if (query.exec(QString("select count(*) from search where lang='%1';").arg(lang)) && query.next()) {
        int icount = query.value(0).toInt();
        qCDebug(app) << "Found" << icount << "entries for language:" << lang;

        //如果当前语言不存在
        if (0 == icount) {
            qCWarning(app) << "No entries found for language:" << lang << "- falling back to default";

            //藏语维语使用简体中文
            if (0 == lang.compare("ug_CN") || 0 == lang.compare("bo_CN")
                    || 0 == lang.compare("zh_HK") || 0 == lang.compare("zh_TW")) {
                lang = "zh_CN";
                qCDebug(app) << "Using Simplified Chinese as fallback language";
            } else {
                lang = "en_US";
                qCDebug(app) << "Using English as fallback language";
            }
        }
    }
    qCDebug(app) << "Using language:" << lang;
    const QString sql =
        QString(kSearchSelectAnchor).replace(":anchor", keyword).replace(":lang", lang);
    qCDebug(app) << "Executing anchor search query:" << sql;
    if (query.exec(sql)) {
        while (query.next() && (result.size() < kResultLimitation)) {
            qCDebug(app) << "handleSearchAnchor===> " << query.value(0).toString() << strlistApp;
            //只将当前预装应用中的内容输出。
            if (strlistApp.contains(query.value(0).toString())/* || query.value(0).toString().contains("video-guide")*/) {
                //搜索结果优先显示应用名称
                if (query.value(3) == "h0") {
                    result.prepend(SearchAnchorResult {
                        query.value(0).toString(),
                        query.value(1).toString(),
                        query.value(2).toString(),
                        query.value(3).toString(),
                    });
                    qCDebug(app) << "handleSearchAnchor===> " << query.value(0).toString() << "h0";
                } else {
                    result.append(SearchAnchorResult {
                        query.value(0).toString(),
                        query.value(1).toString(),
                        query.value(2).toString(),
                        query.value(3).toString(),
                    });
                    qCDebug(app) << "handleSearchAnchor===> " << query.value(0).toString() << "not h0";
                }
            }
        }
    } else {
        qCCritical(app) << "Failed to select anchor:" << query.lastError().text();
    }
    qCDebug(app) << "Anchor search completed:"
               << "keyword:" << keyword
               << "results:" << result.size();
    emit this->searchAnchorResult(keyword, result);
}

/**
 * @brief insertHighlight
 * @param srcString 传入文本，其中内容没有img内容
 * @param keyword 关键字
 * @return
 * 在传入的文本中的搜索关键字加入<span>标签，用以js解析显示高亮
 */
QString SearchDb::insertHighlight(QString srcString, QString keyword)
{
    qCDebug(app) << "insertHighlight===>" << srcString << keyword;
    QString resultString = srcString;
    int currIndex = 0;

    QString highlightStart = "<span class='highlight'>";
    QString highlightEnd = "</span>";

    do {
        QString substr = resultString.mid(currIndex, keyword.length());
        if (substr.toLower() == keyword.toLower()) {
            resultString.insert(currIndex, highlightStart);
            currIndex += highlightStart.length() + keyword.length();
            resultString.insert(currIndex, highlightEnd);
            currIndex += highlightEnd.length();
        } else {
            ++currIndex;
        }

    } while (currIndex <= resultString.length() - keyword.length());

    qCDebug(app) << "insertHighlight===>" << resultString;
    return resultString;
}

/**
 * @brief SearchDb::highlightKeyword
 * @param srcString 搜索结果文本内容
 * @param keyword   搜索关键字
 * @return 返回添加高亮标签之后文本内容
 * 处理搜索结果，为搜索结果中的关键字添加高亮标签， <img>国片内容进行过滤不添加高亮
 */
QString SearchDb::highlightKeyword(QString srcString, QString keyword)
{
    qCDebug(app) << "highlightKeyword===>" << srcString << keyword;
    QString substrImgStart = "";
    QString substrImgEnd = "";

    QStringList strList;
    QList<int> strEndIndexList;

    const QString imgStartString = "<img";
    const QString imgEndString = "\">";

    int currIndex = 0;
    int startSubStringIndex = 0;
    int findImgIndex = 0;
    const int imgStartLen = imgStartString.length();
    const int imgEndLen = imgEndString.length();

    QString highlightString = "";
    //搜索结果中不包含图片，文字添加标签后返回
    if (!srcString.contains(imgStartString)) {
        //添加<span>标签
        highlightString = insertHighlight(srcString, keyword);
        qCDebug(app) << "highlightKeyword===>" << highlightString;
        return highlightString;
    }

    do {
        substrImgStart = srcString.mid(currIndex, imgStartLen);
        if (substrImgStart == imgStartString) {
            findImgIndex = currIndex;
        }

        substrImgEnd = srcString.mid(currIndex, imgEndLen);
        if (substrImgEnd == imgEndString) {
            startSubStringIndex = currIndex + imgEndLen;
            strEndIndexList.append(startSubStringIndex);
            highlightString.append(srcString.mid(findImgIndex, startSubStringIndex - findImgIndex));
        }

        if (findImgIndex > startSubStringIndex) {
            QString findStr =
                srcString.mid(startSubStringIndex, findImgIndex - startSubStringIndex);
            qCDebug(app) << "findStr===>" << findStr;
            if (!strList.contains(findStr)) {
                strList.append(findStr);

                QString hightLightStr = findStr;
                //添加<SPAN>标签
                hightLightStr = insertHighlight(hightLightStr, keyword);
                highlightString.append(hightLightStr);
                qCDebug(app) << "highlightKeyword===>" << highlightString;
            }
        }
        ++currIndex;

    } while (currIndex <= srcString.length());

    if (!strEndIndexList.isEmpty()) {
        int lastImgEndIndex = strEndIndexList.last();

        QString lastStr = srcString.mid(lastImgEndIndex, srcString.length() - lastImgEndIndex);
        qCDebug(app) << "lastStr===>" << lastStr;
        if (lastStr.length() > 0) {
            strList.append(lastStr);

            QString hightLightStr = lastStr;
            //添加<SPAN>标签
            hightLightStr = insertHighlight(hightLightStr, keyword);
            highlightString.append(hightLightStr);
            qCDebug(app) << "highlightKeyword===>" << highlightString;
        }
    }

    qCDebug(app) << "highlightKeyword===>" << highlightString;
    if (!strList.join("").contains(keyword, Qt::CaseInsensitive)) {
        qCDebug(app) << "No keyword found in the highlighted string.";
        return "";
    }

    qCDebug(app) << "highlightKeyword===>" << highlightString;
    return highlightString;
}

/**
 * @brief SearchDb::sortSearchList 搜索结果排序, 将含有h0的应用(应用名称含有关键字)放在最前面, 将标题中含有关键字的放在中间.
 * @param appName      应用名称
 * @param anchors      标题名称
 * @param anchorIds    标题Hash值
 * @param contents     内容
 * @param bIsTitleHigh 该应用搜索结果中标题是否含有关键字  true: 标题含有关键字  false:标题未含有关键字
 */
void SearchDb::sortSearchList(const QString &appName, const QStringList &anchors, const QStringList &anchorIds, const QStringList &contents, bool bIsTitleHigh)
{
    qCDebug(app) << "sortSearchList===>" << appName << anchors << anchorIds << contents << bIsTitleHigh;
    searchStrct obj;
    obj.appName = appName;
    obj.anchors = anchors;
    obj.anchorIds = anchorIds;
    obj.contents = contents;

    if (anchorIds.contains("h0")) {
        //发送查询结果->SearchManager::searchContentResult->SearchProxy::onContentResult->js
        emit this->searchContentResult(obj.appName, obj.anchors, obj.anchorIds, obj.contents);
        nH0OfList++;
        //        listStruct.insert(0, obj);
        //        nH0OfList++;
        qCDebug(app) << "sortSearchList===>h0";
    } else if (bIsTitleHigh) {
        //        listStruct.insert(nH0OfList, obj);
        listStruct.insert(0, obj);
        qCDebug(app) << "sortSearchList===>title high";
    } else {
        listStruct.append(obj);
        qCDebug(app) << "sortSearchList===>not h0";
    }
    qCDebug(app) << "sortSearchList===> listStruct size:" << listStruct.size();
}

/**
 * @brief SearchDb::omitHighlight 高亮省略处理..
 * @param highLight 高亮字段
 * @param keyword  关键字
 * @note  将内容里IMG部分先全部提取出来,并记录IMG内容以及IMG位置. 剩下部分为处理后的文本内容,对此进行关键字位置判断.
 *        如果关键字位置大于N个字符,则对前N个字符用...替代.
 *        如何进行真实内容的截断:循环判断暂存的IMG的位置,如果关键字位置大于暂存的IGM位置的话,就说明省略部分前方有IMG,
 *        需要将IMG内容长度加入到关键字位置上. 否则,则表示前方没有IMG内容,此时关键字位置就能够拿来用作截断位置.
 */
void SearchDb::omitHighlight(QString &highLight, const QString &keyword)
{
    qCDebug(app) << "omitHighlight===>" << highLight << keyword;
    QString highLightTemp = highLight;
    int nindex = highLightTemp.length();
    const QString imgStartString = "<img";
    const QString imgEndString = "\">";
    int imgStartLeng = imgStartString.length();
    QList<QString> imgList;
    QList<int> imgIndexList;
    int nSearchIndex = nindex - imgStartLeng;
    qCDebug(app) << "omitHighlight===> nSearchIndex:" << nSearchIndex;

    while (nSearchIndex > 0) {
        //判断后N个字符里是否含有img路径..
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
        QRegExp regExp("<img .*");
#else
        QRegularExpression regExp("<img .*");
#endif
        if (highLightTemp.indexOf(regExp, nSearchIndex) != -1) {
            qCDebug(app) << "omitHighlight===> find img";
            int nImgStart = highLightTemp.indexOf(imgStartString, nSearchIndex);
            int nImgEnd = highLightTemp.indexOf(imgEndString, nSearchIndex);
            if (nImgEnd > nImgStart) {
                QString strImg = highLightTemp.mid(nImgStart, nImgEnd - nImgStart);
                imgList.insert(0, strImg);
                imgIndexList.insert(0, nImgStart);
                highLightTemp = highLightTemp.left(nImgStart + 1) + highLightTemp.right(highLightTemp.length() - nImgEnd);
                qCDebug(app) << "omitHighlight===> imgList:" << imgList << "imgIndexList:" << imgIndexList;
            }
        }
        nSearchIndex -= imgStartLeng;
    }
    qCDebug(app) << "omitHighlight===> highLightTemp:" << highLightTemp;

    int keywordIndex = highLightTemp.indexOf(keyword, 0, Qt::CaseInsensitive);
    //暂时用150个字符来判断...后期是否可根据不同语言来分别用不同的长度判断条件..
    if (keywordIndex > 150) {
        int nOmitIndex = keywordIndex - 150;
        qCDebug(app) << "omitHighlight===> nOmitIndex:" << nOmitIndex;

        for (int i = 0; i < imgIndexList.count(); i++) {
            if (nOmitIndex > imgIndexList[i]) {
                nOmitIndex += imgList[i].length();
            }
        }
        highLight = "..." + highLight.mid(nOmitIndex);
    }
    qCDebug(app) << "omitHighlight===> highLight:" << highLight;
}

/**
 * @brief SearchDb::handleSearchContent
 * @param keyword 搜索关键字
 * 根据关键字执行搜索，并对搜索结果进行处理，
 * 通过searchContentResult()信号发送到JS进行显示
 */
void SearchDb::handleSearchContent(const QString &keyword)
{
    qCDebug(app) << keyword;
    Q_ASSERT(p_->db.isOpen());
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QRegExp exp("[|&-]");
    QRegExp exp2("%%");
#else
    QRegularExpression exp("[|&-]");
    QRegularExpression exp2("%%");
#endif
    //屏蔽部分特殊字符，会导致JS层对HTML无法对应替换
    if (keyword.contains(exp)) {
        qCDebug(app) << "Keyword contains special character, search failed";
        //发送未查找到结果->SearchManager::searchContentMismatch->SearchProxy::mismatch->JS
        emit this->searchContentMismatch(keyword);
        return;
    }
    //多个%号查询，查询条件错误
    if (keyword.contains(exp2)) {
        qCWarning(app) << "Invalid search keyword containing multiple %%:" << keyword;
        //发送未查找到结果->SearchManager::searchContentMismatch->SearchProxy::mismatch->JS
        emit this->searchContentMismatch(keyword);
        return;
    }
    qCDebug(app) << "Keyword validation passed, proceeding with search";

    QSqlQuery query(p_->db);
    QString lang = QLocale().name();
    if (query.exec(QString("select count(*) from search where lang='%1';").arg(lang)) && query.next()) {
        qCDebug(app) << "Found" << query.value(0).toInt() << "entries for language:" << lang;
        int icount = query.value(0).toInt();
        //如果当前语言不存在
        if (0 == icount) {
            //藏语维语使用简体中文
            if (0 == lang.compare("ug_CN") || 0 == lang.compare("bo_CN")
                    || 0 == lang.compare("zh_HK") || 0 == lang.compare("zh_TW")) {
                qCDebug(app) << "No entries found for language:" << lang << "- falling back to zh_CN";
                lang = "zh_CN";
            } else {
                qCDebug(app) << "No entries found for language:" << lang << "- falling back to en_US";
                lang = "en_US";
            }
        }
    }
    const QString sql = QString(kSearchSelectContent).replace(":lang", lang).replace(":content", keyword);
    qCDebug(app) << "Executing content search query:" << sql;

    listStruct.clear();
    nH0OfList = 0;
    bool result_empty = true;
    if (query.exec(sql)) {
        qCDebug(app) << "Search content query executed successfully";
        bool bIsTitle = false;
        QString last_app_name = "";
        QStringList anchors;
        QStringList anchorIds;
        QStringList contents;
        QHash<QString, bool> appHasMatchHash;
        QElapsedTimer tm;
        tm.start();
        while (query.next()) {
            const QString app_name = query.value(0).toString();
            const QString anchor = query.value(1).toString();
            const QString anchorId = query.value(2).toString();
            const QString content = query.value(3).toString();
            if (!strlistApp.contains(app_name) && !app_name.contains("video-guide")) {
                qCDebug(app) << "app_name not in strlistApp, continue";
                continue;
            }

            QString tmpContent = content;
            tmpContent = tmpContent.replace("alt>", ">");
            tmpContent = tmpContent.replace("\" >", "\">");

            QString highlightContent = highlightKeyword(tmpContent, keyword);
            qCDebug(app) << "highlightContent:" << highlightContent;

            if (app_name.contains("video-guide")) {
                qCDebug(app) << "app_name contains video-guide, continue";
                highlightContent = tmpContent;
            }


            //如果关键字在img路径中,返回后退出本次循环.
            if (highlightContent.isEmpty() && !anchor.contains(keyword)) {
                qCDebug(app) << "highlightContent is empty, continue";
                continue;
            }

            //去除jpg文件, 影响页面格式.
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
            //旧结构
            QRegExp expJpg("<img src=\\\"jpg.*>");
            expJpg.setMinimal(true);
            //新结构
            QRegExp expFit("<img src=\\\"fig.*>");
            expFit.setMinimal(true);
            //相对路径
            QRegExp expRelative("<img src=\\\"\.\/fig.*>");
            expRelative.setMinimal(true);
#else
            //旧结构
            QRegularExpression expJpg("<img src=\\\"jpg.*?>");
            //新结构
            QRegularExpression expFit("<img src=\\\"fig.*?>");
            //相对路径
            QRegularExpression expRelative("<img src=\\\"\.\/fig.*?>");
#endif

            highlightContent.remove(expFit);
            highlightContent.remove(expJpg);
            highlightContent.remove(expRelative);

            //处理内容是否省略..
            omitHighlight(highlightContent, keyword);

            if (app_name == last_app_name) {
                qCDebug(app) << "app_name is same with last_app_name, continue";
                anchors.append(anchor);
                anchorIds.append(anchorId);
                contents.append(highlightContent);
                if (anchor.contains(keyword)) {
                    qCDebug(app) << "anchor contains keyword, bIsTitle = true";
                    bIsTitle = true;
                }
            } else {
                qCDebug(app) << "app_name is different with last_app_name, sortSearchList";
                if (!last_app_name.isEmpty()) {
                    sortSearchList(last_app_name, anchors, anchorIds, contents, bIsTitle);
                    anchors.clear();
                    anchorIds.clear();
                    contents.clear();
                    bIsTitle = false;
                    qCDebug(app) << "sortSearchList done";
                }
                anchors.append(anchor);
                anchorIds.append(anchorId);
                contents.append(highlightContent);
                if (anchor.contains(keyword)) {
                    qCDebug(app) << "anchor contains keyword, bIsTitle = true";
                    bIsTitle = true;
                }
                last_app_name = app_name;
            }
        }
        qCInfo(app) << "search content time:" << tm.elapsed();
        if (!last_app_name.isEmpty()) {
            sortSearchList(last_app_name, anchors, anchorIds, contents, bIsTitle);
        }
        for (searchStrct obj : listStruct) {
            if (result_empty) {
                result_empty = false;
                qCDebug(app) << "result_empty = false";
            }
            //发送查询结果->SearchManager::searchContentResult->SearchProxy::onContentResult->js
            emit this->searchContentResult(obj.appName, obj.anchors, obj.anchorIds, obj.contents);
        }
    } else {
        qCCritical(app) << "Failed to select contents:" << query.lastError().text();
    }

    if (result_empty && 0 == nH0OfList) {
        qCDebug(app) << "searchContentMismatch";
        //发送未查找到结果->SearchManager::searchContentMismatch->SearchProxy::mismatch->JS
        emit this->searchContentMismatch(keyword);
    }
    qCDebug(app) << "handleSearchContent done";
}

/**
 * @brief SearchDb::updateFileTimeEntry
 * @param appName   应用名
 * @param lang      语言
 * @param dataTime  文件时间信息
 * index.md 时间信息插入fileTime表,
 * 先找到相应数据删除行，再执行插入操作
 */
void SearchDb::insertFilesTimeEntry(const QStringList &listMdPath, const QStringList &listDataTime)
{
    Q_ASSERT(p_->db.isOpen());
    qCDebug(app) << "insertFilesTimeEntry";

    QSqlQuery query(p_->db);
    bool preok = query.prepare(kfileTimeDeleteEntryByApp);
    if (!preok) {
        qCCritical(app) << "Failed to prepare kSearchDeleteEntryByApp:" << query.lastError().text();
        return;
    }
    query.bindValue(0, listMdPath);
    if (!query.execBatch()) {
        qCCritical(app) << "Failed to delete fileTime entry:" << query.lastError().text();
        return;
    }

    if (!p_->db.transaction()) {
        qWarning() << "Failed to start db transaction!";
        return;
    }
    preok = query.prepare(kfileTimeInsertEntry);
    if (!preok) {
        qCCritical(app) << "Failed to prepare kfileTimeInsertEntry:" << query.lastError().text();
        return;
    }
    query.bindValue(0, listMdPath);
    query.bindValue(1, listDataTime);
    bool ok = query.execBatch();

    if (!ok) {
        p_->db.rollback();
        qCCritical(app) << "Failed to insert fileTime " << query.lastError().text();
    } else {
        bool ret = p_->db.commit();
        qCCritical(app) << "insert fileTime" << ret;
    }
    qCDebug(app) << "insertFilesTimeEntry done";
}

/**
 * @brief SearchDb::deleteFilesTimeEntry
 * @param listMdPath  删除文件路径列表
 */
void SearchDb::deleteFilesTimeEntry(const QStringList &listMdPath)
{
    Q_ASSERT(p_->db.isOpen());
    qCDebug(app) << "deleteFilesTimeEntry";

    QSqlQuery query(p_->db);
    bool preok = query.prepare(kfileTimeDeleteEntryByApp);
    if (!preok) {
        qCCritical(app) << "Failed to prepare kfileTimeDeleteEntryByApp:" << query.lastError().text();
        return;
    }
    query.bindValue(0, listMdPath);
    bool ok = query.execBatch();
    if (!ok) {
        p_->db.rollback();
        qCCritical(app) << "Failed to delete fileTime entry:" << query.lastError().text();
    } else {
        p_->db.commit();
        qCCritical(app) << "delete fileInfo : mdPath = " << listMdPath;
    }
    qCDebug(app) << "deleteFilesTimeEntry done";
}

/**
 * @brief SearchDb::selectAllFileTime 查询所有的文件更新时间信息
 * @return
 */
QMap<QString, QString> SearchDb::selectAllFileTime()
{
    Q_ASSERT(p_->db.isOpen());
    qCDebug(app) << "selectAllFileTime";
    QMap<QString, QString> mapRet;
    QSqlQuery query(p_->db);
    if (query.exec(kfileTimeSelectAll)) {
        qCDebug(app) << "selectAllFileTime query executed successfully";
        while (query.next()) {
            mapRet.insert(query.value(0).toString(), query.value(1).toString());
        }
    }
    qCDebug(app) << "selectAllFileTime done";
    return mapRet;
}

/**
 * @brief SearchDb::getAllApp
 * 获取系统中存在帮助手册的应用列表
 */
void SearchDb::getAllApp()
{
    qCDebug(app) << "getAllApp";
    strlistApp = Utils::getSystemManualList();
    qCDebug(app) << "getAllApp done";
}

void SearchDb::updateDb()
{
    qCDebug(app) << "updateDb";
    //删除本地数据库
    QString dbdir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.local/share/deepin/deepin-manual/search.db" ;
    QDir dir(dbdir);
    if (!dir.exists()) {
        qCCritical(app) << "db path not exist!" << dbdir;
    }

    dir.remove(dbdir);
    //重新加载数据库，创建新表
    createTable();
    qCDebug(app) << "updateDb done";
}

void SearchDb::createTable()
{
    qCDebug(app) << "createTable";
    initDb();
    initSearchTable();
    initTimeTable();
    qCDebug(app) << "createTable done";
}
