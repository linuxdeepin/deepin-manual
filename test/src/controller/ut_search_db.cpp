#include "ut_search_db.h"

#include "controller/search_db.h"

#include <QSqlQuery>
#include <QSqlDatabase>

namespace dman {

ut_search_db_test::ut_search_db_test()
{
}


/**

TEST_F(ut_search_db_test, initSearchTable)
{

    QString databasePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    databasePath += "/.local/share/deepin/deepin-manual/search.db";
    QString cmd = "cp ";
    cmd += databasePath;
    cmd += " ";
    cmd += DMAN_SEARCH_CREATE_DB_PATH;
    qDebug() << Q_FUNC_INFO <<  cmd;
    QProcess pro;
    pro.start(cmd);

    QString strCreateDbPath = DMAN_SEARCH_CREATE_DB_PATH;
    QDir dir(strCreateDbPath);
    if (!dir.exists()) {
        dir.mkpath(strCreateDbPath);
    }
    strCreateDbPath += "/search.db";
    SearchDb sd;
    sd.initDb();
    sd.initSearchTable();
    qCritical() << "Failed to drop search table";

    QString cmdCp = "cp ";
    cmdCp += DMAN_SEARCH_CREATE_DB_PATH;
    cmdCp += "/search.db ";
    cmdCp += databasePath;
    cmdCp += "/.local/share/deepin/deepin-manual";
    QProcess pro2;
    pro2.start(cmdCp);
    qDebug() << Q_FUNC_INFO << cmdCp;
//}

TEST_F(ut_search_db_test, initSearchTable2)
{
    /*
        QString strCreateDbPath = DMAN_SEARCH_CREATE_DB_PATH;
        QDir dir(strCreateDbPath);
        if (!dir.exists()) {
            dir.mkpath(strCreateDbPath);
        }
        strCreateDbPath += "/search.db";

        //匿名命名空间 在其它文件中无法使用。。。
        QProcess p;
        p.execute("rm " + strCreateDbPath);
        p.waitForFinished();
        SearchDb sd;
        sd.initDb(strCreateDbPath);
        sd.initSearchTable();
        qCritical() << "Failed to initialize search table";
    */
//}

//TEST_F(ut_search_db_test, initSearchTable3)
//{
/*
    QString strCreateDbPath = DMAN_SEARCH_CREATE_DB_PATH;
    QDir dir(strCreateDbPath);
    if (!dir.exists()) {
        dir.mkpath(strCreateDbPath);
    }
    strCreateDbPath += "/search.db";

    QProcess p;
    p.execute("rm " + strCreateDbPath);
    p.waitForFinished();

    const char kSearchTableSchema[] =
        "CREATE TABLE IF NOT EXISTS search "
        "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "appName TEXT,"
        "lang INTEGER,"
        "anchor TEXT,"
        "anchorInitial TEXT,"
        "anchorSpell TEXT,"
        "anchorId TEXT,"
        "content TEXT)";
    const char kSearchIndexSchema[] =
        "CREATE INDEX IF NOT EXISTS search_idx "
        "ON search (id, appName, lang, anchor)";
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(strCreateDbPath);
    if (db.open()) {
        QSqlQuery query(db);
        ASSERT_TRUE(query.exec(kSearchTableSchema));
        //        ASSERT_TRUE(query.exec(kSearchIndexSchema));
    }
//    p.execute("chmod 444 " + strCreateDbPath);
//    p.waitForFinished();
    SearchDb sd;
    sd.initDb(strCreateDbPath);
    sd.initSearchTable();

//sql语句在匿名空间内，无法修改，无法进度以下条件；
qCritical() << "Failed to initialize search table";

*/
//}

//TEST_F(ut_search_db_test, addSearchEntry)
//{
//    QString system = "professional";
//    QString app_name = "chineseime";
//    QString lang = "zh_CN";

//    QStringList anchors{"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
//    QStringList anchorInitialList{"gs", "syrm", "qhsrf", "ztl", "sxsz", "jbsz", "mrsz"};
//    QStringList anchorSpellList{"gaishu", "shiyongrumen", "qiehuanshurufa", "zhuangtailan", "shuxingshezhi", "jibenshezhi", "morenshezhi"};
//    QStringList anchorIdList{"h0", "h0", "h0", "h0", "h0", "h0", "h0"};
//    QStringList contents{"00系统会预装中文输入法，单击右下角托盘上的",
//                         "01进入输入法配置页面。单击 输入法，选择中文输入法",
//                         "02托盘中输入法图标变为<img，则表示切换成功。",
//                         "03系统会预装中文输入法，单击右下角托盘上的",
//                         "04seime/zh_CN/icon/onboard.svg",
//                         "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法图标变为<img src=",
//                         "06/usr/share/deepin-manual/manual-assets/professional/chineseime/zh_CN/icon/inputer.svg"
//                        };
//    SearchDb sd;
//    sd.initDb();
//    sd.initSearchTable();
//    sd.addSearchEntry(app_name, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);

//    QString databasePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
//    QString cmdCp = "cp ";
//    cmdCp += DMAN_SEARCH_CREATE_DB_PATH;
//    cmdCp += "/search.db ";
//    cmdCp += databasePath;
//    cmdCp += "/.local/share/deepin/deepin-manual";
//    QProcess pro2;
//    pro2.start(cmdCp);

//}

//TEST_F(ut_search_db_test, addSearchEntry2)
//{
//    QString system = "professional";
//    QString app_name = "chineseime";
//    QString lang = "zh_CN";

//    QStringList anchors{"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
//    QStringList anchorInitialList{"gs", "syrm", "qhsrf", "ztl", "sxsz", "jbsz", "mrsz"};
//    QStringList anchorSpellList{"gaishu", "shiyongrumen", "qiehuanshurufa", "zhuangtailan", "shuxingshezhi", "jibenshezhi", "morenshezhi"};
//    QStringList anchorIdList{"h0", "h0", "h0", "h0", "h0", "h0"}; //六项
//    QStringList contents{"00系统会预装中文输入法，单击右下角托盘上的",
//                         "01进入输入法配置页面。单击 输入法，选择中文输入法",
//                         "02托盘中输入法图标变为<img，则表示切换成功。",
//                         "03系统会预装中文输入法，单击右下角托盘上的",
//                         "04seime/zh_CN/icon/onboard.svg",
//                         "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法图标变为<img src=",
//                         "06/usr/share/deepin-manual/manual-assets/professional/chineseime/zh_CN/icon/inputer.svg"
//                        };

//    SearchDb sd;
//    sd.initDb();
//    sd.initSearchTable();
//    sd.addSearchEntry(app_name, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);

//    QString databasePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
//    QString cmdCp = "cp ";
//    cmdCp += DMAN_SEARCH_CREATE_DB_PATH;
//    cmdCp += "/search.db ";
//    cmdCp += databasePath;
//    cmdCp += "/.local/share/deepin/deepin-manual";
//    QProcess pro2;
//    pro2.start(cmdCp);


//    QProcess p;
//    p.start("cp /usr/share/deepin-manual/manual-assets/professional/search.db "
//            "/home/kevin_w/project/qtest/build-manual-unknown-Debug/src/dbPath/");
//    p.close();
//}

/*
TEST_F(ut_search_db_test, addSearchEntry3)
{
    QString system = "professional";
    QString app_name = "chineseime";
    QString lang = "zh_CN";

    QStringList anchors{"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
    QStringList anchorInitialList{"gs", "syrm", "qhsrf", "ztl", "sxsz", "jbsz", "mrsz"};
//    QStringList anchorSpellList{"gaishu", "shiyongrumen", "qiehuanshurufa", "zhuangtailan", "shuxingshezhi", "jibenshezhi", "morenshezhi"};
    QStringList anchorSpellList{};
    QStringList anchorIdList{"h0", "h0", "h0", "h0", "h0", "h0", "h1"};
    QStringList contents{"00系统会预装中文输入法，单击右下角托盘上的",
                         "01进入输入法配置页面。单击 输入法，选择中文输入法",
                         "02托盘中输入法图标变为<img，则表示切换成功。",
                         "03系统会预装中文输入法，单击右下角托盘上的",
                         "04seime/zh_CN/icon/onboard.svg",
                         "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法图标变为<img src=",
                         "06/usr/share/deepin-manual/manual-assets/professional/chineseime/zh_CN/icon/inputer.svg"
                        };

    SearchDb sd;
    QString strCreateDbPath = DMAN_SEARCH_CREATE_DB_PATH;

    QDir dir(strCreateDbPath);
    dir.setFilter(QDir::Files);
    if (!dir.exists()) {
        dir.mkpath(strCreateDbPath);
    }
    strCreateDbPath += "/search.db";
    QFileInfo fileInfo(strCreateDbPath);
    sd.initDb(strCreateDbPath);
    if (!fileInfo.isFile()) {
        sd.initSearchTable();
    }

    sd.addSearchEntry(system, app_name, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
}
*/



//TEST_F(ut_search_db_test, handleSearchAchor)
//{
//    QString system = "professional";
//    QString app_name = "chineseime";
//    QString lang = "zh_CN";

//    QStringList anchors{"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
//    QStringList anchorInitialList{"gs", "syrm", "qhsrf", "ztl", "sxsz", "jbsz", "mrsz"};
//    QStringList anchorSpellList{"gaishu", "shiyongrumen", "qiehuanshurufa", "zhuangtailan", "shuxingshezhi", "jibenshezhi", "morenshezhi"};
//    QStringList anchorIdList{"h0", "h0", "h0", "h0", "h0", "h0", "h0"};
//    QStringList contents{"00系统会预装中文输入法，单击右下角托盘上的",
//                         "01进入输入法配置页面。单击 输入法，选择中文输入法",
//                         "02托盘中输入法图标变为<img，则表示切换成功。",
//                         "03系统会预装中文输入法，单击右下角托盘上的",
//                         "04seime/zh_CN/icon/onboard.svg",
//                         "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法图标变为<img src=",
//                         "06/usr/share/deepin-manual/manual-assets/professional/chineseime/zh_CN/icon/inputer.svg"
//                        };
//    SearchDb sd;
//    sd.initDb();
//    sd.initSearchTable();
//    sd.addSearchEntry(app_name, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
//    sd.handleSearchAnchor("概述");


//    QString databasePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
//    QString cmdCp = "cp ";
//    cmdCp += DMAN_SEARCH_CREATE_DB_PATH;
//    cmdCp += "/search.db ";
//    cmdCp += databasePath;
//    cmdCp += "/.local/share/deepin/deepin-manual";
//    QProcess pro2;
//    pro2.start(cmdCp);
//}

//TEST_F(ut_search_db_test, insertHighlight)
//{
//全局函数，无法测试
//QString insertHighlight(QString srcString, QString keyword)
//}

//TEST_F(ut_search_db_test, highlightKeyword)
//{
//    QString srcString = "系统会预装中文输入法";
//    QString srcKeyword = "中文";
//    QString value = "系统会预装<span class='highlight'>中文</span>输入法";
//    SearchDb sd;
//    QString tmp = sd.highlightKeyword(srcString, srcKeyword);
//    ASSERT_EQ(tmp, value);
//}

//TEST_F(ut_search_db_test, getAllApp)
//{
//SearchDb sd;
//    sd.getAllApp();
//    ASSERT_GT(sd.strlistApp.size(), 20);
//}

//TEST_F(ut_search_db_test, sortSearchList)
//{
//    QString appName = "chineseime";
//    QStringList anchors{"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
//    QStringList anchorIdList{"h0", "h0", "h0", "h0", "h0", "h0", "h0"};
//    QStringList contents{"00系统会预装中文输入法，单击右下角托盘上的",
//                         "01进入输入法配置页面。单击 输入法，选择中文输入法",
//                         "02托盘中输入法图标变为<img，则表示切换成功。",
//                         "03系统会预装中文输入法，单击右下角托盘上的",
//                         "04seime/zh_CN/icon/onboard.svg",
//                         "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法图标变为<img src=",
//                         "06/usr/share/deepin-manual/manual-assets/professional/chineseime/zh_CN/icon/inputer.svg"
//                        };
////    sortSearchList(const QString &appName, const QStringList &anchors,
////                   const QStringList &anchorIds,
////                   const QStringList &contents, bool bIsTitleHigh

//    SearchDb sd;
//    sd.sortSearchList(appName, anchors, anchorIdList, contents, false);
//}

//TEST_F(ut_search_db_test, sortSearchList2)
//{
//    QString appName = "chineseime";
//    QStringList anchors{"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
//    QStringList anchorIdList{"h1", "h1", "h1", "h1", "h1", "h1", "h1"};
//    QStringList contents{"00系统会预装中文输入法，单击右下角托盘上的",
//                         "01进入输入法配置页面。单击 输入法，选择中文输入法",
//                         "02托盘中输入法图标变为<img，则表示切换成功。",
//                         "03系统会预装中文输入法，单击右下角托盘上的",
//                         "04seime/zh_CN/icon/onboard.svg",
//                         "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法图标变为<img src=",
//                         "06/usr/share/deepin-manual/manual-assets/professional/chineseime/zh_CN/icon/inputer.svg"
//                        };


//    SearchDb sd;
//    sd.sortSearchList(appName, anchors, anchorIdList, contents, true);
//}

//TEST_F(ut_search_db_test, sortSearchList3)
//{
//    QString appName = "chineseime";
//    QStringList anchors{"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
//    QStringList anchorIdList{"h1", "h1", "h1", "h1", "h1", "h1", "h1"};
//    QStringList contents{"00系统会预装中文输入法，单击右下角托盘上的",
//                         "01进入输入法配置页面。单击 输入法，选择中文输入法",
//                         "02托盘中输入法图标变为<img，则表示切换成功。",
//                         "03系统会预装中文输入法，单击右下角托盘上的",
//                         "04seime/zh_CN/icon/onboard.svg",
//                         "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法图标变为<img src=",
//                         "06/usr/share/deepin-manual/manual-assets/professional/chineseime/zh_CN/icon/inputer.svg"
//                        };


//    SearchDb sd;
//    sd.sortSearchList(appName, anchors, anchorIdList, contents, false);
//}

//TEST_F(ut_search_db_test, onitHighlight)
//{
//    QString srcString = "01进入输入法配置页面。单击 输入法，选择中文输入法"
//                        "02托盘中输入法图g，则表示切换成功。"
//                        "03系统会预装中文输入法，单击右下角托盘上的"
//                        "04seime/zh_CN/icon/onboard.svg"
//                        "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法"
//                        "01进入输入法配置页面。单击 输入法，选择中文输入法"
//                        "02托盘中输入法图g，则表示切换成功。"
//                        "03系统会预装中文输入法，单击右下角托盘上的"
//                        "04seime/zh_CN/icon/onboard.svg"
//                        "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法"
//                        "选择中文输入法，托盘中输入法图标变为<img src=\"/usr/share/deepin-manual/manual-assets/professional/chineseime/zh_CN/icon/inputer.svg\">，则表"
//                        ;
//    QString srcKeyword = "图标";
//    QString value = "系统会预装<span class='highlight'>中文</span>输入法";
//    SearchDb sd;
//    sd.omitHighlight(srcString, srcKeyword);
//}

//TEST_F(ut_search_db_test, handleSearchContent)
//{
//    SearchDb sd;
//    sd.initDb();
//    sd.handleSearchContent("中文");

//    QString databasePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
//    QString cmdCp = "cp ";
//    cmdCp += DMAN_SEARCH_CREATE_DB_PATH;
//    cmdCp += "/search.db ";
//    cmdCp += databasePath;
//    cmdCp += "/.local/share/deepin/deepin-manual";
//    QProcess pro2;
//    pro2.start(cmdCp);
//}

//TEST_F(ut_search_db_test, handleSearchContent2)
//{
/*
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
SearchDb sd;
sd.initDb();
sd.handleSearchContent("应用名称");

//数据库表名恢复
QSqlDatabase db2;
db2 = QSqlDatabase::addDatabase("QSQLITE");
db2.setDatabaseName(strCreateDbPath);
if (db2.open()) {
    QSqlQuery query(db2);
//       ASSERT_TRUE(query.exec(kReductionTableName));
}


QString databasePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
QString cmdCp = "cp ";
cmdCp += DMAN_SEARCH_CREATE_DB_PATH;
cmdCp += "/search.db ";
cmdCp += databasePath;
cmdCp += "/.local/share/deepin/deepin-manual";
QProcess pro2;
pro2.start(cmdCp);

*/
//}

//TEST_F(ut_search_db_test, initTimeTable)
//{
////    SearchDb sd;
////    sd.initTimeTable();
//}

//TEST_F(ut_search_db_test, insertFilesTimeEntry)
//{
//    QStringList listMdPath;
//    QStringList listDataTime;
//    listMdPath << "aaaa" << "A";
//    listDataTime << "0011" << "0022";

//    SearchDb sd;
//    sd.initDb();
//    sd.initTimeTable();
//    sd.insertFilesTimeEntry(listMdPath, listDataTime);

//    QString databasePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
//    QString cmdCp = "cp ";
//    cmdCp += DMAN_SEARCH_CREATE_DB_PATH;
//    cmdCp += "/search.db ";
//    cmdCp += databasePath;
//    cmdCp += "/.local/share/deepin/deepin-manual";
//    QProcess pro2;
//    pro2.start(cmdCp);
//}

//TEST_F(ut_search_db_test, deleteFilesTimeEntry)
//{
//    QStringList listMdPath;
//    listMdPath << "cccc" << "C";
//    SearchDb sd;
//    sd.initDb();
//    sd.deleteFilesTimeEntry(listMdPath);


//    QString databasePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
//    QString cmdCp = "cp ";
//    cmdCp += DMAN_SEARCH_CREATE_DB_PATH;
//    cmdCp += "/search.db ";
//    cmdCp += databasePath;
//    cmdCp += "/.local/share/deepin/deepin-manual";
//    QProcess pro2;
//    pro2.start(cmdCp);
//}

//TEST_F(ut_search_db_test, selectAllFileTime)
//{
//    QMap<QString, QString> map;
//    QStringList listMdPath;
//    QStringList listDataTime;
//    listMdPath << "aaaa" << "A";
//    listDataTime << "0011" << "0022";

//    SearchDb sd;
//    sd.initDb();
//    sd.insertFilesTimeEntry(listMdPath, listDataTime);
//    map = sd.selectAllFileTime();
//    ASSERT_EQ(map["aaaa"], "0011");
//    ASSERT_EQ(map["A"], "0022");


//    QString databasePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
//    QString cmdCp = "cp ";
//    cmdCp += DMAN_SEARCH_CREATE_DB_PATH;
//    cmdCp += "/search.db ";
//    cmdCp += databasePath;
//    cmdCp += "/.local/share/deepin/deepin-manual";
//    QProcess pro2;
//    pro2.start(cmdCp);



//}


}
