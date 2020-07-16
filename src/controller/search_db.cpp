/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "controller/search_db.h"

#include <DLog>
#include <DSysInfo>

#include <QDateTime>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace dman {

namespace {

const char kSearchDropTable[] = "DROP TABLE IF EXISTS search";

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
    "AND (t1.anchor LIKE '%:anchor%' "
    "OR t1.anchorSpell LIKE '%:anchor%' "
    "OR t1.anchorInitial LIKE '%:anchor%') --case insensitive";

const char kSearchSelectContent[] =
    "SELECT appName, anchor, anchorId, content "
    "FROM search "
    "WHERE lang = ':lang' AND "
    "content LIKE '%:content%' --case insensitive";

const int kResultLimitation = INT_MAX;

}  // namespace

struct SearchDbPrivate {
    QSqlDatabase db;
};

SearchDb::SearchDb(QObject *parent)
    : QObject(parent)
    , p_(new SearchDbPrivate())
{
    qRegisterMetaType<SearchAnchorResult>("SearchAnchorResult");
    qRegisterMetaType<SearchAnchorResultList>("SearchAnchorResultList");
    qRegisterMetaType<SearchContentResult>("SearchContentResult");
    qRegisterMetaType<SearchContentResultList>("SearchContentResultList");

    this->initConnections();
}

SearchDb::~SearchDb()
{
    if (p_ != nullptr) {
        if (p_->db.isOpen()) {
            p_->db.close();
        }
        delete p_;
        p_ = nullptr;
    }
}

void SearchDb::initConnections()
{
    connect(this, &SearchDb::initDbAsync, this, &SearchDb::initDb);
    connect(this, &SearchDb::searchAnchor, this, &SearchDb::handleSearchAnchor);
    connect(this, &SearchDb::searchContent, this, &SearchDb::handleSearchContent);
    connect(this, &SearchDb::installApps, this, &SearchDb::handleInstallApps);
}

void SearchDb::initDb(const QString &db_path)
{
    qDebug() << "initDb database path is------------------>:" << db_path << endl;
    p_->db = QSqlDatabase::addDatabase("QSQLITE");
    p_->db.setDatabaseName(db_path);
    if (!p_->db.open()) {
        qCritical() << "Failed to open search db:" << db_path;
        return;
    }
}

void SearchDb::initSearchTable()
{
    QSqlQuery query(p_->db);
    if (!query.exec(kSearchDropTable)) {
        qCritical() << "Failed to drop search table";
        return;
    }

    if (!query.exec(kSearchTableSchema)) {
        qCritical() << "Failed to initialize search table:" << query.lastError().text();
        return;
    }
    if (!query.exec(kSearchIndexSchema)) {
        qCritical() << "Failed to create index for search table" << query.lastError().text();
        return;
    }
}

void SearchDb::addSearchEntry(const QString &app_name, const QString &lang,
                              const QStringList &anchors, const QStringList &anchorInitialList,
                              const QStringList &anchorSpellList, const QStringList &anchorIdList,
                              const QStringList &contents)
{
    Q_ASSERT(p_->db.isOpen());
    Q_ASSERT(anchors.length() == contents.length());
    qDebug() << "addSearchEntry()" << app_name << lang << anchors;  // << contents;

    QString strManualPath = "";
    int nType = Dtk::Core::DSysInfo::deepinType();
    if (Dtk::Core::DSysInfo::DeepinServer == (Dtk::Core::DSysInfo::DeepinType)nType) {
        strManualPath += "/server";
    } else if (Dtk::Core::DSysInfo::DeepinPersonal == (Dtk::Core::DSysInfo::DeepinType)nType) {
        strManualPath += "/personal";
    } else {
        if (Dtk::Core::DSysInfo::isCommunityEdition()) {
            strManualPath += "/community";
        } else {
            strManualPath += "/professional";
        }
    }

    QStringList newContents = contents;
    for (int i = 0; i < contents.size(); i++) {
        QString content = contents.at(i);
        content = content.replace("icon/", DMAN_INSTALL_DB_PATH + strManualPath +
                                  "/" + app_name + "/" + lang + "/icon/");
        newContents.replace(i, content);
    }

    if (anchors.length() != newContents.length() || anchors.length() != anchorIdList.length()) {
        qCritical() << "anchor list and contents mismatch:" << anchors.length()
                    << newContents.length();
        return;
    }

    QSqlQuery query(p_->db);
    query.prepare(kSearchDeleteEntryByApp);
    query.bindValue(0, app_name);
    query.bindValue(1, lang);
    if (!query.exec()) {
        qCritical() << "Failed to delete search entry:" << query.lastError().text();
        return;
    }

    if (!p_->db.transaction()) {
        qWarning() << "Failed to start db transaction!";
        return;
    }
    query.prepare(kSearchInsertEntry);
    QStringList app_names;
    QStringList lang_list;
    for (int i = 0; i < anchors.length(); ++i) {
        app_names.append(app_name);
        lang_list.append(lang);
    }
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
        qCritical() << "Failed to insert search entry:" << query.lastError().text();
    } else {
        p_->db.commit();
    }
}

void SearchDb::handleSearchAnchor(const QString &keyword)
{
    qDebug() << Q_FUNC_INFO << keyword;
    Q_ASSERT(p_->db.isOpen());

    SearchAnchorResultList result;

    QSqlQuery query(p_->db);
    const QString lang = QLocale().name();
    const QString sql =
        QString(kSearchSelectAnchor).replace(":anchor", keyword).replace(":lang", lang);
    qDebug() << "handleSearchAnchor sql is:" << sql;
    if (query.exec(sql)) {
        while (query.next() && (result.size() < kResultLimitation)) {
            //只将当前预装应用中的内容输出。
            if (strlistApp.contains(query.value(0).toString())) {
                //搜索结果优先显示应用名称
                if (query.value(3) == "h0") {
                    result.prepend(SearchAnchorResult {
                        query.value(0).toString(),
                        query.value(1).toString(),
                        query.value(2).toString(),
                        query.value(3).toString(),
                    });
                } else {
                    result.append(SearchAnchorResult {
                        query.value(0).toString(),
                        query.value(1).toString(),
                        query.value(2).toString(),
                        query.value(3).toString(),
                    });
                }
            }
        }
    } else {
        qCritical() << "Failed to select anchor:" << query.lastError().text();
    }
    qDebug() << "result size:" << result.size() << keyword;
    emit this->searchAnchorResult(keyword, result);
}

QString insertHighlight(QString srcString, QString keyword)
{
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

    return resultString;
}

QString SearchDb::highlightKeyword(QString srcString, QString keyword)
{
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

    if (!srcString.contains(imgStartString)) {
        highlightString = insertHighlight(srcString, keyword);
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
            if (!strList.contains(findStr)) {
                strList.append(findStr);

                QString hightLightStr = findStr;
                hightLightStr = insertHighlight(hightLightStr, keyword);
                highlightString.append(hightLightStr);
            }
        }
        ++currIndex;

    } while (currIndex <= srcString.length());

    if (!strEndIndexList.isEmpty()) {
        int lastImgEndIndex = strEndIndexList.last();

        QString lastStr = srcString.mid(lastImgEndIndex, srcString.length() - lastImgEndIndex);
        if (lastStr.length() > 0) {
            strList.append(lastStr);

            QString hightLightStr = lastStr;
            hightLightStr = insertHighlight(hightLightStr, keyword);
            highlightString.append(hightLightStr);
        }
    }

    if (!strList.join("").contains(keyword, Qt::CaseInsensitive)) {
        return "";
    }

    return highlightString;
}

void SearchDb::handleSearchContent(const QString &keyword)
{
    qDebug() << Q_FUNC_INFO << keyword;

    Q_ASSERT(p_->db.isOpen());

    QSqlQuery query(p_->db);
    const QString lang = QLocale().name();
    const QString sql =
        QString(kSearchSelectContent).replace(":lang", lang).replace(":content", keyword);

    bool result_empty = true;
    if (query.exec(sql)) {
        QString last_app_name;
        QStringList anchors;
        QStringList anchorIds;
        QStringList contents;
        QHash<QString, bool> appHasMatchHash;
        while (query.next()) {
            const QString app_name = query.value(0).toString();
            const QString anchor = query.value(1).toString();
            const QString anchorId = query.value(2).toString();
            const QString content = query.value(3).toString();
            if (!strlistApp.contains(app_name)) {
                continue;
            }

            QString tmpContent = content;
            tmpContent = tmpContent.replace("alt>", ">");
            tmpContent = tmpContent.replace("\" >", "\">");

            QString highlightContent = highlightKeyword(tmpContent, keyword);

            //remove jpg src
            QRegExp exp("<img src=\\\"jpg.*>");
            exp.setMinimal(true);
            highlightContent.remove(exp);

            if (highlightContent.length() > 0) {
                appHasMatchHash.insert(app_name, true);
            }

            if (app_name == last_app_name) {
                if (highlightContent.length() > 0) {
                    result_empty = false;
                    anchors.append(anchor);
                    anchorIds.append(anchorId);
                    contents.append(highlightContent);
                }
            } else {
                if (!last_app_name.isEmpty() && appHasMatchHash.value(last_app_name) &&
                        contents.size() > 0) {
                    result_empty = false;
                    qDebug() << Q_FUNC_INFO << "emit searchContentResult()" << last_app_name << " " << contents.length();
                    emit this->searchContentResult(last_app_name, anchors, anchorIds, contents);
                }

                anchors.clear();
                anchorIds.clear();
                contents.clear();

                if (highlightContent.length() > 0) {
                    result_empty = false;
                    last_app_name = app_name;
                    anchors.append(anchor);
                    anchorIds.append(anchorId);
                    contents.append(highlightContent);
                }
            }
        }

        // Last record.
        if (!result_empty && contents.size() > 0) {
            result_empty = false;
            qDebug() << Q_FUNC_INFO << "emit searchContentResult() last record"
                     << contents.length();
            emit this->searchContentResult(last_app_name, anchors, anchorIds, contents);
        }
    } else {
        qCritical() << "Failed to select contents:" << query.lastError().text();
    }

    if (result_empty) {
        qDebug() << "searchContentMismatch";
        emit this->searchContentMismatch(keyword);
    }
}

void SearchDb::handleInstallApps(const QStringList &strlistApps)
{
    strlistApp = strlistApps;
}

}  // namespace dman
