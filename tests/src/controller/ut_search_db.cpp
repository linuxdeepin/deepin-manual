#include "ut_search_db.h"

#include "controller/search_db.h"
#include "src/third-party/stub/stub.h"
#include "base/utils.h"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QFile>

static int m_count = 0;

ut_search_db_test::ut_search_db_test()
{
}

void ut_search_db_test::SetUp()
{
    sd = new SearchDb();
}

void ut_search_db_test::TearDown()
{
    delete sd;
}

bool ut_search_db_test::fromLocalFileRestore()
{
    QString databasePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    databasePath += "/.local/share/deepin/deepin-manual/search.db";
    QFile::remove(databasePath);

    QString databaseBackup = databasePath + "backup";

    QFile fp(databaseBackup);
    if (fp.exists()) {
        if (QFile::copy(databaseBackup, databasePath))
            QFile::remove(databaseBackup);
        return true;
    } else {
        qDebug() << "search.dbbackup no exists...";
        return false;
    }
}

bool ut_search_db_test::fromLocalFileBackup()
{
    QString databasePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    databasePath += "/.local/share/deepin/deepin-manual/search.db";

    QString databaseBackup = databasePath + "backup";
    QFile fp(databasePath);
    if (fp.exists()) {
        if (QFile::copy(databasePath, databaseBackup))
            QFile::remove(databasePath);
        return true;
    } else {
        qDebug() << "search.db no exists...";
        return false;
    }
}

bool stub_open()
{
    return true;
}

TEST_F(ut_search_db_test, SearchDb)
{
    Stub s;
    s.set(ADDR(QSqlDatabase, isOpen), stub_open);
    SearchDb db;
    s.reset(ADDR(QSqlDatabase, isOpen));
}

TEST_F(ut_search_db_test, initSearchTable)
{
    QString databasePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    databasePath += "/.local/share/deepin/deepin-manual/search.db";
    QString cmd = "cp ";
    cmd += databasePath;
    cmd += " ";
    cmd += DMAN_SEARCH_CREATE_DB_PATH;
    qDebug() << Q_FUNC_INFO << cmd;
    QProcess pro;
    pro.start(cmd);

    QString strCreateDbPath = DMAN_SEARCH_CREATE_DB_PATH;
    QDir dir(strCreateDbPath);
    if (!dir.exists()) {
        dir.mkpath(strCreateDbPath);
    }
    strCreateDbPath += "/search.db";

    sd->initDb();
    sd->initSearchTable();
    qCritical() << "Failed to drop search table";

    QString cmdCp = "cp ";
    cmdCp += DMAN_SEARCH_CREATE_DB_PATH;
    cmdCp += "/search.db ";
    cmdCp += databasePath;
    cmdCp += "/.local/share/deepin/deepin-manual";
    QProcess pro2;
    pro2.start(cmdCp);
    qDebug() << Q_FUNC_INFO << cmdCp;
}

bool stub_exec()
{
    m_count++;
    if (m_count == 1 || m_count == 3) {
        return false;
    } else {
        return true;
    }
}

TEST_F(ut_search_db_test, initSearchTable2)
{
    QString strCreateDbPath = DMAN_SEARCH_CREATE_DB_PATH;
    QDir dir(strCreateDbPath);
    if (!dir.exists()) {
        dir.mkpath(strCreateDbPath);
    }

    sd->initDb();
    Stub st;
    QSqlQuery ql;
    st.set((bool (QSqlQuery::*)(const QString &))ADDR(QSqlQuery, exec), stub_exec);
    sd->initSearchTable();
    qCritical() << "Failed to initialize search table";
}

TEST_F(ut_search_db_test, initSearchTable3)
{
    QString strCreateDbPath = DMAN_SEARCH_CREATE_DB_PATH;
    QDir dir(strCreateDbPath);
    if (!dir.exists()) {
        dir.mkpath(strCreateDbPath);
    }
    sd->initDb();
    Stub st;
    QSqlQuery ql;
    st.set((bool (QSqlQuery::*)(const QString &))ADDR(QSqlQuery, exec), stub_exec);
    sd->initSearchTable();
    qCritical() << "Failed to initialize search table";
}

TEST_F(ut_search_db_test, addSearchEntry)
{
    this->fromLocalFileBackup();

    QString system = "professional";
    QString app_name = "chineseime";
    QString lang = "zh_CN";

    QStringList anchors {"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
    QStringList anchorInitialList {"gs", "syrm", "qhsrf", "ztl", "sxsz", "jbsz", "mrsz"};
    QStringList anchorSpellList {"gaishu", "shiyongrumen", "qiehuanshurufa", "zhuangtailan", "shuxingshezhi", "jibenshezhi", "morenshezhi"};
    QStringList anchorIdList {"h0", "h0", "h0", "h0", "h0", "h0", "h0"};
    QStringList contents {"00系统会预装中文输入法，单击右下角托盘上的",
                          "01进入输入法配置页面。单击 输入法，选择中文输入法",
                          "02托盘中输入法图标变为<img，则表示切换成功。",
                          "03系统会预装中文输入法，单击右下角托盘上的",
                          "04seime/zh_CN/icon/onboard.svg",
                          "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法图标变为<img src=",
                          "06/usr/share/deepin-manual/manual-assets/professional/chineseime/zh_CN/icon/inputer.svg"};

    sd->initDb();
    sd->initSearchTable();
    sd->addSearchEntry(app_name, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);

    this->fromLocalFileRestore();
}

bool stub_exec2()
{
    return false;
}
TEST_F(ut_search_db_test, initTimeTable)
{
    Stub st;
    st.set((bool (QSqlQuery::*)(const QString &))ADDR(QSqlQuery, exec), stub_exec2);
    sd->initTimeTable();
}

TEST_F(ut_search_db_test, handleSearchAchor)
{
    this->fromLocalFileBackup();

    QString system = "professional";
    QString app_name = "chineseime";
    QString lang = "zh_CN";

    QStringList anchors {"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
    QStringList anchorInitialList {"gs", "syrm", "qhsrf", "ztl", "sxsz", "jbsz", "mrsz"};
    QStringList anchorSpellList {"gaishu", "shiyongrumen", "qiehuanshurufa", "zhuangtailan", "shuxingshezhi", "jibenshezhi", "morenshezhi"};
    QStringList anchorIdList {"h0", "h0", "h0", "h0", "h0", "h0", "h0"};
    QStringList contents {"00系统会预装中文输入法，单击右下角托盘上的",
                          "01进入输入法配置页面。单击 输入法，选择中文输入法",
                          "02托盘中输入法图标变为<img，则表示切换成功。",
                          "03系统会预装中文输入法，单击右下角托盘上的",
                          "04seime/zh_CN/icon/onboard.svg",
                          "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法图标变为<img src=",
                          "06/usr/share/deepin-manual/manual-assets/professional/chineseime/zh_CN/icon/inputer.svg"};

    sd->initDb();
    sd->initSearchTable();
    sd->addSearchEntry(app_name, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
    sd->handleSearchAnchor("概述");

    this->fromLocalFileRestore();
}

TEST_F(ut_search_db_test, insertHighlight)
{
    QString str = sd->insertHighlight("abcd", "a");
}

TEST_F(ut_search_db_test, deleteSearchInfo)
{
    sd->initDb();
    QStringList applist;
    applist.append("abcd");
    QStringList applistlang;
    applistlang.append("a");
    sd->deleteSearchInfo(applist, applistlang);
    this->fromLocalFileRestore();
}

TEST_F(ut_search_db_test, highlightKeyword)
{
    QString srcString = "系统会预装中文输入法";
    QString srcKeyword = "中文";
    QString value = "系统会预装<span class='highlight'>中文</span>输入法";
    QString tmp = sd->highlightKeyword(srcString, srcKeyword);
    ASSERT_EQ(tmp, value);
}

TEST_F(ut_search_db_test, highlightKeyword2)
{
    QString srcString = "<img系统会预装中文输入法.jpg";
    QString srcKeyword = "中文";
    QString value = "系统会预装<span class='highlight'>中文</span>输入法";
    QString tmp = sd->highlightKeyword(srcString, srcKeyword);
    //    ASSERT_EQ(tmp, value);
}

TEST_F(ut_search_db_test, sortSearchList)
{
    QString appName = "chineseime";
    QStringList anchors {"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
    QStringList anchorIdList {"h0", "h0", "h0", "h0", "h0", "h0", "h0"};
    QStringList contents {"00系统会预装中文输入法，单击右下角托盘上的",
                          "01进入输入法配置页面。单击 输入法，选择中文输入法",
                          "02托盘中输入法图标变为<img，则表示切换成功。",
                          "03系统会预装中文输入法，单击右下角托盘上的",
                          "04seime/zh_CN/icon/onboard.svg",
                          "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法图标变为<img src=",
                          "06/usr/share/deepin-manual/manual-assets/professional/chineseime/zh_CN/icon/inputer.svg"};
    //    sortSearchList(const QString &appName, const QStringList &anchors,
    //                   const QStringList &anchorIds,
    //                   const QStringList &contents, bool bIsTitleHigh

    sd->sortSearchList(appName, anchors, anchorIdList, contents, false);
}

TEST_F(ut_search_db_test, sortSearchList2)
{
    QString appName = "chineseime";
    QStringList anchors {"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
    QStringList anchorIdList {"h1", "h1", "h1", "h1", "h1", "h1", "h1"};
    QStringList contents {"00系统会预装中文输入法，单击右下角托盘上的",
                          "01进入输入法配置页面。单击 输入法，选择中文输入法",
                          "02托盘中输入法图标变为<img，则表示切换成功。",
                          "03系统会预装中文输入法，单击右下角托盘上的",
                          "04seime/zh_CN/icon/onboard.svg",
                          "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法图标变为<img src=",
                          "06/usr/share/deepin-manual/manual-assets/professional/chineseime/zh_CN/icon/inputer.svg"};

    sd->sortSearchList(appName, anchors, anchorIdList, contents, true);
}

TEST_F(ut_search_db_test, sortSearchList3)
{
    QString appName = "chineseime";
    QStringList anchors {"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
    QStringList anchorIdList {"h1", "h1", "h1", "h1", "h1", "h1", "h1"};
    QStringList contents {"00系统会预装中文输入法，单击右下角托盘上的",
                          "01进入输入法配置页面。单击 输入法，选择中文输入法",
                          "02托盘中输入法图标变为<img，则表示切换成功。",
                          "03系统会预装中文输入法，单击右下角托盘上的",
                          "04seime/zh_CN/icon/onboard.svg",
                          "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法图标变为<img src=",
                          "06/usr/share/deepin-manual/manual-assets/professional/chineseime/zh_CN/icon/inputer.svg"};

    sd->sortSearchList(appName, anchors, anchorIdList, contents, false);
}

TEST_F(ut_search_db_test, onitHighlight)
{
    QString srcString = "01进入输入法配置页面。单击 输入法，选择中文输入法"
                        "02托盘中输入法图g，则表示切换成功。"
                        "03系统会预装中文输入法，单击右下角托盘上的"
                        "04seime/zh_CN/icon/onboard.svg"
                        "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法"
                        "01进入输入法配置页面。单击 输入法，选择中文输入法"
                        "02托盘中输入法图g，则表示切换成功。"
                        "03系统会预装中文输入法，单击右下角托盘上的"
                        "04seime/zh_CN/icon/onboard.svg"
                        "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法"
                        "选择中文输入法，托盘中输入法图标变为<img src=\"/usr/share/deepin-manual/manual-assets/professional/chineseime/zh_CN/icon/inputer.svg\">，则表";
    QString srcKeyword = "图标";
    QString value = "系统会预装<span class='highlight'>中文</span>输入法";

    sd->omitHighlight(srcString, srcKeyword);
}

TEST_F(ut_search_db_test, handleSearchContent)
{
    sd->initDb();
    sd->initSearchTable();
    sd->initTimeTable();
    sd->handleSearchContent("&");
}

TEST_F(ut_search_db_test, handleSearchContent2)
{
    this->fromLocalFileBackup();
    QString strCreateDbPath = DMAN_SEARCH_CREATE_DB_PATH;
    QDir dir(strCreateDbPath);
    if (!dir.exists()) {
        dir.mkpath(strCreateDbPath);
    }
    strCreateDbPath += "/search2.db";

    const QString kDeleteAnchorID = "UPDATE search "
                                    "SET lang = null "
                                    "WHERE lang = 'zh_CN'";
    const QString kReNameTable = "ALTER TABLE 'search' RENAME TO 'search-test'";
    const QString kReductionTableName = "ALTER TABLE 'search-test' RENAME TO 'search'";

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(strCreateDbPath);
    if (db.open()) {
        QSqlQuery query(db);
        //        ASSERT_TRUE(query.exec(kDeleteAnchorID));
        //      ASSERT_TRUE(query.exec(kReNameTable));
    }

    sd->initDb();
    sd->initSearchTable();
    sd->initTimeTable();
    sd->handleSearchContent("应用名称");

    //数据库表名恢复
    QSqlDatabase db2;
    db2 = QSqlDatabase::addDatabase("QSQLITE");
    db2.setDatabaseName(strCreateDbPath);
    if (db2.open()) {
        QSqlQuery query(db2);
        //       ASSERT_TRUE(query.exec(kReductionTableName));
    }

    this->fromLocalFileRestore();
}

TEST_F(ut_search_db_test, insertFilesTimeEntry)
{
    this->fromLocalFileBackup();
    QStringList listMdPath;
    QStringList listDataTime;
    listMdPath << "aaaa"
               << "A";
    listDataTime << "0011"
                 << "0022";

    sd->initDb();
    sd->initTimeTable();
    sd->insertFilesTimeEntry(listMdPath, listDataTime);

    this->fromLocalFileRestore();
}

TEST_F(ut_search_db_test, deleteFilesTimeEntry)
{
    this->fromLocalFileBackup();
    QStringList listMdPath;
    listMdPath << "cccc"
               << "C";
    QStringList listTime;
    listTime << "001"
             << "002";

    sd->initDb();
    sd->initTimeTable();
    sd->insertFilesTimeEntry(listMdPath, listTime);
    sd->deleteFilesTimeEntry(listMdPath);

    this->fromLocalFileRestore();
}

TEST_F(ut_search_db_test, selectAllFileTime)
{
    this->fromLocalFileBackup();

    QMap<QString, QString> map;
    QStringList listMdPath;
    QStringList listDataTime;
    listMdPath << "aaaa"
               << "A";
    listDataTime << "0011"
                 << "0022";

    sd->initDb();
    sd->initTimeTable();
    sd->insertFilesTimeEntry(listMdPath, listDataTime);
    map = sd->selectAllFileTime();
    //    ASSERT_EQ(map["aaaa"], "0011");
    //    ASSERT_EQ(map["A"], "0022");

    this->fromLocalFileRestore();
}

TEST_F(ut_search_db_test, selectAllFileTime2)
{
    this->fromLocalFileBackup();

    QMap<QString, QString> map;
    QStringList listMdPath;
    QStringList listDataTime;
    listMdPath << "aaaa"
               << "A";
    listDataTime << "0011"
                 << "0022";

    sd->initDb();
    sd->initTimeTable();
    sd->insertFilesTimeEntry(listMdPath, listDataTime);
    map = sd->selectAllFileTime();
    //    ASSERT_EQ(map["aaaa"], "0011");
    //    ASSERT_EQ(map["A"], "0022");

    this->fromLocalFileRestore();
}

TEST_F(ut_search_db_test, getAllApp)
{
    QStringList list = Utils::getSystemManualList();
    sd->getAllApp();
    ASSERT_EQ(list, sd->strlistApp);
}
