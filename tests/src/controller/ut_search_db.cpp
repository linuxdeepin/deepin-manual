// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_search_db.h"
#include "controller/search_db.h"
#include "base/utils.h"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QFile>

static int m_count = 0;
static int m_count1 = 0;
Stub *ut_search_db_test::stt = nullptr;

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

int execcnt = 0;

bool stub_open()
{
    return true;
}

bool stub_openFalse()
{
    return false;
}

bool stub_openFalseCnt()
{
    if (execcnt == 0) {
        execcnt++;
        return true;
    }
    execcnt = 0;
    return false;
}

QString stub_mkdir(const QString & path)
{
    return "/detest";
}

QString ut_search_db_test::stub_writableLocation(QStandardPaths::StandardLocation type)
{
    return "/tmp";
}

QString firstremovedbfile()
{
    QString dbdir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append("/.local/share/deepin/deepin-manual");
    QString databasePath1 = dbdir.append("/search.db");
    qWarning() << "db file:" << databasePath1;
    QFile::remove(databasePath1);

    QFile file(databasePath1);

    if (!file.exists()) {
        qWarning() << "delete file success";
    }

    return databasePath1;
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

TEST_F(ut_search_db_test, SearchDb)
{
//    Stub s;
//    s.set(ADDR(QSqlDatabase, isOpen), stub_open);
//    s.set(ADDR(QStandardPaths, writableLocation), ADDR(ut_search_db_test, stub_writableLocation));

//    SearchDb db;
//    ASSERT_GT(db.strlistApp.size(), 0);
}

TEST_F(ut_search_db_test, initSearchTable)
{
    Stub s;
    s.set(ADDR(QStandardPaths, writableLocation), ADDR(ut_search_db_test, stub_writableLocation));

    QString databasePath1 = firstremovedbfile();

    sd->initDb();

    QFile file(databasePath1);
    ASSERT_TRUE(file.exists());
    qWarning() << "create db file success";
    sd->initSearchTable();

    QString sql = "select count(1) from sqlite_master where type='table' and name ='search' order by name";

    //创建数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString(QUuid::WithoutBraces));
    db.setDatabaseName(databasePath1);
    db.open();

    QSqlQuery query(db);

    query.exec(sql);
    query.next();
    int icount = query.value(0).toInt();
    query.next();
    ASSERT_EQ(icount, 1);
    query.exec("drop table search");
    query.next();

    s.set((bool (QSqlQuery::*)(const QString &))ADDR(QSqlQuery, exec), stub_openFalse);
    sd->initSearchTable();
    s.reset((bool (QSqlQuery::*)(const QString &))ADDR(QSqlQuery, exec));

    s.set((bool (QSqlQuery::*)(const QString &))ADDR(QSqlQuery, exec), stub_openFalseCnt);
    sd->initSearchTable();
    s.reset((bool (QSqlQuery::*)(const QString &))ADDR(QSqlQuery, exec));

    query.exec(sql);
    query.next();
    icount = query.value(0).toInt();
    query.next();

    ASSERT_EQ(icount, 0);
    db.close();
}

TEST_F(ut_search_db_test, initDb)
{
    Stub s;
    s.set((bool (QSqlDatabase::*)())ADDR(QSqlDatabase, open), stub_openFalse);
    sd->initDb();
    s.set(ADDR(Utils, mkMutiDir), stub_mkdir);
    sd->initDb();
    QString filepath = "/detest/search.db";
    QFile file(filepath);
    ASSERT_FALSE(file.exists());
}

TEST_F(ut_search_db_test, addSearchEntry)
{
    Stub st;
    st.set(ADDR(QStandardPaths, writableLocation), ADDR(ut_search_db_test, stub_writableLocation));
    QString databasePath1 = firstremovedbfile();

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
    QString mdPath = "/usr/share/deepin-manual/manual-assets/application/dde-file-manager/file-manager/zh_CN/d_file-manager.md";

    sd->initDb();
    sd->initSearchTable();
    sd->addSearchEntry(app_name, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);

    QString sql = "select count(1) from search where appname ='chineseime' and content like '00系统会预装中文输入法，单击右下角托盘上的%' and anchorid='h0' and lang = 'zh_CN'";

    //创建数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString(QUuid::WithoutBraces));
    db.setDatabaseName(databasePath1);
    db.open();

    QSqlQuery query(db);

    query.exec(sql);
    query.next();
    int icount = query.value(0).toInt();
    query.next();

    ASSERT_EQ(icount, 1);

    QString app_name1 = "dde";
    sd->addSearchEntry(app_name1, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);

    sql = "select count(1) from search where appname ='dde' and content like '00系统会预装中文输入法，单击右下角托盘上的' and anchorid='h0' and lang = 'zh_CN'";

    query.exec(sql);
    query.next();
    icount = query.value(0).toInt();
    query.next();
    ASSERT_EQ(icount, 1);

    sd->addSearchEntry(app_name, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents, mdPath);

    sql = "select count(1) from search where appname ='chineseime' and content like '00系统会预装中文输入法，单击右下角托盘上的%' and anchorid='h0' and lang = 'zh_CN'";
    query.exec(sql);
    query.next();
    icount = query.value(0).toInt();
    query.next();
    ASSERT_EQ(icount, 1);

    QStringList anchorIdList1 {"h0", "h0", "h0", "h0", "h0", "h0"};

    sd->addSearchEntry(app_name, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList1, contents);

    sql = "select count(1) from search where appname ='chineseime' and anchorid='h0' and lang = 'zh_CN'";
    query.exec(sql);
    query.next();
    icount = query.value(0).toInt();
    query.next();
    ASSERT_EQ(icount, 7);

    query.exec("delete from search");

    QSqlQuery ql;
    st.set((bool (QSqlQuery::*)())ADDR(QSqlQuery, execBatch), stub_openFalse);

    sd->addSearchEntry(app_name, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);

    sql = "select count(1) from search where appname ='chineseime' and anchorid='h0' and lang = 'zh_CN'";
    query.exec(sql);
    query.next();
    icount = query.value(0).toInt();
    query.next();
    ASSERT_EQ(icount, 0);

    st.reset((bool (QSqlQuery::*)())ADDR(QSqlQuery, execBatch));
    sd->addSearchEntry(app_name, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);

    st.set((bool (QSqlQuery::*)())ADDR(QSqlQuery, exec), stub_openFalse);
    sd->addSearchEntry(app_name, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);

    sql = "select count(1) from search where appname ='chineseime' and content like '00系统会预装中文输入法，单击右下角托盘上的%' and anchorid='h0' and lang = 'zh_CN'";
    query.exec(sql);
    query.next();
    icount = query.value(0).toInt();
    query.next();
    ASSERT_EQ(icount, 1);

    st.reset((bool (QSqlQuery::*)())ADDR(QSqlQuery, exec));

    st.set((bool (QSqlDatabase::*)())ADDR(QSqlDatabase, transaction), stub_openFalse);
    sd->addSearchEntry(app_name, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);

    sql = "select count(1) from search where appname ='chineseime' and content like '00系统会预装中文输入法，单击右下角托盘上的%' and anchorid='h0' and lang = 'zh_CN'";
    query.exec(sql);
    query.next();
    icount = query.value(0).toInt();
    query.next();
    ASSERT_EQ(icount, 0);

    st.reset((bool (QSqlQuery::*)())ADDR(QSqlQuery, exec));

    m_count = 0;
    st.set((bool (QSqlQuery::*)())ADDR(QSqlQuery, prepare), stub_exec);
    sd->addSearchEntry(app_name, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);

    sql = "select count(1) from search where appname ='chineseime' and content like '00系统会预装中文输入法，单击右下角托盘上的%' and anchorid='h0' and lang = 'zh_CN'";
    query.exec(sql);
    query.next();
    icount = query.value(0).toInt();
    query.next();
    st.reset((bool (QSqlQuery::*)())ADDR(QSqlQuery, prepare));
    st.reset((bool (QSqlDatabase::*)())ADDR(QSqlDatabase, transaction));
    ASSERT_EQ(icount, 0);

    db.close();
}

bool stub_exec2()
{
    return false;
}
TEST_F(ut_search_db_test, initTimeTable)
{
    Stub st;
    st.set(ADDR(QStandardPaths, writableLocation), ADDR(ut_search_db_test, stub_writableLocation));
    QString databasePath1 = firstremovedbfile();

    sd->initDb();

    QFile file(databasePath1);
    if (file.exists()) {
        qWarning() << "db file exists";
    }

    sd->initTimeTable();

    QString sql = "select count(1) from sqlite_master where type='table' and name ='filetime' order by name";

    //创建数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString(QUuid::WithoutBraces));
    db.setDatabaseName(databasePath1);
    db.open();

    QSqlQuery query(db);

    query.exec(sql);
    query.next();
    int icount = query.value(0).toInt();
    query.next();
    ASSERT_EQ(icount, 1);
    qWarning() << "db table filetime exisit";

    st.set((bool (QSqlQuery::*)(const QString &))ADDR(QSqlQuery, exec), stub_exec2);
    sd->initTimeTable();
    query.exec(sql);
    query.next();
    icount = query.value(0).toInt();
    query.next();
    ASSERT_EQ(icount, 0);
}

QString stub_LocalNamezh_HK()
{
    return "zh_HK";
}

QString stub_LocalNamezh_TW()
{
    return "zh_TW";
}

QString stub_LocalNameug_CN()
{
    return "ug_CN";
}

QString stub_LocalNameug_eu()
{
    return "en_US";
}

QString stub_LocalNameug_bo_CN()
{
    return "bo_CN";
}

QString stub_LocalNameug_zh_CN()
{
    return "zh_CN";
}

QVariant ut_search_db_test::stub_value(int num) const
{
    if(num == 3)
    {
        return "h0";
    }

    return "dde";
}


QVariant ut_search_db_test::stub_valuenoh0(int num) const
{
    if(num == 3)
    {
        return "h1";
    }

    return "dde";
}


QVariant ut_search_db_test::stub_nextfalse(int num) const
{
    if(num == 3)
    {
        if(m_count1 == 4)
        {
            delete stt;
            stt = new Stub;
            stt->set((bool (QSqlQuery::*)())ADDR(QSqlQuery, next), stub_exec2);
        }
        m_count1++;

    }
    else if(num == 0 && m_count1 == 0)
    {
        m_count1++;
        return 1;
    }
    else if(num == 0 && m_count1 == 1)
    {
        m_count1++;
        return "dde";
    }
    else if(num == 0 && m_count1 == 3)
    {
        m_count1++;
        return "dde";
    }
    return 0;
}


bool ut_search_db_test::stub_transactionture() const
{
    stt->set((bool (QSqlQuery::*)())ADDR(QSqlQuery, execBatch), stub_openFalse);
    return true;
}

QVariant ut_search_db_test::stub_valueh3(int num) const
{
    if(num == 3)
    {
        return "h1";
    }

    return "dde";
}


TEST_F(ut_search_db_test, handleSearchAchor)
{
    Stub s;
    s.set(ADDR(QStandardPaths, writableLocation), ADDR(ut_search_db_test, stub_writableLocation));

    QSignalSpy spy(sd, SIGNAL(searchAnchorResult(const QString,
                                                 const SearchAnchorResultList &)));

    QString system = "professional";
    QString app_name = "dde";
    QString lang = "zh_CN";

    QStringList anchors {"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
    QStringList anchorInitialList {"gs", "syrm", "qhsrf", "ztl", "sxsz", "jbsz", "mrsz"};
    QStringList anchorSpellList {"gaishu", "shiyongrumen", "qiehuanshurufa", "zhuangtailan", "shuxingshezhi", "jibenshezhi", "morenshezhi"};
    QStringList anchorIdList {"h0", "h1", "h2", "h3", "h4", "h5", "h6"};
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
    sd->addSearchEntry(app_name, "en_US", anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
    sd->addSearchEntry(app_name, "zh_TW", anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
    sd->addSearchEntry(app_name, "zh_HK", anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
    sd->addSearchEntry(app_name, "bo_CN", anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);

    s.set(ADDR(QLocale, name), stub_LocalNameug_CN);
    sd->handleSearchAnchor("切换");
    ASSERT_EQ(spy.count(), 1);
    s.reset(ADDR(QLocale, name));

    s.set(ADDR(QLocale, name), stub_LocalNameug_eu);
    sd->handleSearchAnchor("切换");
    ASSERT_EQ(spy.count(), 2);
    s.reset(ADDR(QLocale, name));

    s.set(ADDR(QLocale, name), stub_LocalNamezh_TW);
    sd->handleSearchAnchor("概述");
    ASSERT_EQ(spy.count(), 3);
    s.reset(ADDR(QLocale, name));

    s.set(ADDR(QLocale, name), stub_LocalNamezh_HK);
    sd->handleSearchAnchor("概述");
    ASSERT_EQ(spy.count(), 4);

    s.reset(ADDR(QLocale, name));
    s.set(ADDR(QLocale, name), stub_LocalNameug_bo_CN);
    sd->handleSearchAnchor("概述");
    ASSERT_EQ(spy.count(), 5);
    s.reset(ADDR(QLocale, name));

    m_count = 0;
    s.set((QVariant (QSqlQuery::*)(int num) const)ADDR(QSqlQuery, value), ADDR(ut_search_db_test, stub_value));
    s.set((bool (QSqlQuery::*)())ADDR(QSqlQuery, next), stub_exec);
    sd->handleSearchAnchor("概述");
    ASSERT_EQ(spy.count(), 6);
    s.reset((QVariant (QSqlQuery::*)(int num) const)ADDR(QSqlQuery, value));
    s.reset((bool (QSqlQuery::*)())ADDR(QSqlQuery, next));

    m_count = 0;
    s.set((QVariant (QSqlQuery::*)(int num) const)ADDR(QSqlQuery, value), ADDR(ut_search_db_test, stub_valuenoh0));
    s.set((bool (QSqlQuery::*)())ADDR(QSqlQuery, next), stub_exec);
    sd->handleSearchAnchor("概述");
    ASSERT_EQ(spy.count(), 7);
    s.reset((QVariant (QSqlQuery::*)(int num) const)ADDR(QSqlQuery, value));
    s.reset((bool (QSqlQuery::*)())ADDR(QSqlQuery, next));

    s.set((QVariant (QSqlQuery::*)(int num) const)ADDR(QSqlQuery, value), ADDR(ut_search_db_test, stub_valueh3));
    sd->handleSearchAnchor("概述");
    ASSERT_EQ(spy.count(), 8);
    s.reset((QVariant (QSqlQuery::*)(int num) const)ADDR(QSqlQuery, value));

    s.set((bool (QSqlQuery::*)(const QString &))ADDR(QSqlQuery, exec), stub_openFalse);
    sd->handleSearchAnchor("概述");
    ASSERT_EQ(spy.count(), 9);
    s.reset((bool (QSqlQuery::*)(const QString &))ADDR(QSqlQuery, exec));
}

TEST_F(ut_search_db_test, insertHighlight)
{
    QString str = sd->insertHighlight("abcd", "a");
    ASSERT_EQ(str, "<span class='highlight'>a</span>bcd");
}

TEST_F(ut_search_db_test, deleteSearchInfo)
{
    Stub s;
    s.set(ADDR(QStandardPaths, writableLocation), ADDR(ut_search_db_test, stub_writableLocation));
    QString dbdir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append("/.local/share/deepin/deepin-manual");
    QString databasePath1 = dbdir.append("/search.db");
    QFile::remove(databasePath1);

    QString system = "professional";
    QString app_name = "dde";
    QString lang = "zh_CN";

    QStringList anchors {"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
    QStringList anchorInitialList {"gs", "syrm", "qhsrf", "ztl", "sxsz", "jbsz", "mrsz"};
    QStringList anchorSpellList {"gaishu", "shiyongrumen", "qiehuanshurufa", "zhuangtailan", "shuxingshezhi", "jibenshezhi", "morenshezhi"};
    QStringList anchorIdList {"h0", "h1", "h2", "h3", "h4", "h5", "h6"};
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
    sd->addSearchEntry(app_name, "en_US", anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
    sd->addSearchEntry(app_name, "zh_TW", anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
    sd->addSearchEntry(app_name, "zh_HK", anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
    sd->addSearchEntry(app_name, "bo_CN", anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);

    QStringList applist;
    applist.append("dde");
    QStringList applistlang;
    applistlang.append("zh_CN");
    applistlang.append("zh_HK");
    sd->deleteSearchInfo(applist, applistlang);

    QString sql = "select count(1) from search where appname ='dde' and lang = 'zh_CN'";

    //创建数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString(QUuid::WithoutBraces));
    db.setDatabaseName(databasePath1);
    db.open();

    QSqlQuery query(db);

    query.exec(sql);
    query.next();
    int icount = query.value(0).toInt();
    query.next();
    ASSERT_EQ(icount, 0);

    s.set((bool (QSqlQuery::*)())ADDR(QSqlQuery, execBatch), stub_openFalse);
    sd->deleteSearchInfo(applist, applistlang);
    sql = "select count(1) from search where appname ='dde' and lang = 'zh_HK'";
    query.exec(sql);
    query.next();
    icount = query.value(0).toInt();
    query.next();
    ASSERT_TRUE(icount > 0);
    db.close();
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
    ASSERT_EQ(tmp, "");
    srcString = "\"><img系统会预装中文输入法.jpg>";
    tmp = sd->highlightKeyword(srcString, srcKeyword);
    ASSERT_NE(tmp, value);

    srcString = "\">12344555<img系统会预装中文输入法.jpg>";
    tmp = sd->highlightKeyword(srcString, srcKeyword);
    ASSERT_NE(tmp, value);
}

TEST_F(ut_search_db_test, sortSearchList)
{
    Stub s;
    s.set(ADDR(QStandardPaths, writableLocation), ADDR(ut_search_db_test, stub_writableLocation));
    QString dbdir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append("/.local/share/deepin/deepin-manual");
    QString databasePath1 = dbdir.append("/search.db");
    QFile::remove(databasePath1);
    sd->initDb();
    sd->initSearchTable();

    QString appName = "chineseime";
    QString lang = "zh_CN";
    QStringList anchors {"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
    QStringList anchorInitialList {"gs", "syrm", "qhsrf", "ztl", "sxsz", "jbsz", "mrsz"};
    QStringList anchorSpellList {"gaishu", "shiyongrumen", "qiehuanshurufa", "zhuangtailan", "shuxingshezhi", "jibenshezhi", "morenshezhi"};
    QStringList anchorIdList {"h0", "h1", "h2", "h3", "h4", "h5", "h6"};
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

    sd->addSearchEntry(appName, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
    sd->addSearchEntry(appName, "en_US", anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
    sd->addSearchEntry(appName, "zh_TW", anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
    sd->addSearchEntry(appName, "zh_HK", anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
    sd->addSearchEntry(appName, "bo_CN", anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);

    sd->sortSearchList(appName, anchors, anchorIdList, contents, false);
    ASSERT_TRUE(sd->nH0OfList > 0);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QStringList anchorIdList1 {"h2", "h1", "h2", "h3", "h4", "h5", "h6"};
    sd->sortSearchList(appName, anchors, anchorIdList1, contents, false);
    ASSERT_EQ(sd->listStruct.count(), 1);

    sd->sortSearchList(appName, anchors, anchorIdList1, contents, true);
    ASSERT_EQ(sd->listStruct.count(), 2);
#endif
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

    ASSERT_TRUE(srcString.contains("..."));
}

TEST_F(ut_search_db_test, handleSearchContent)
{
    Stub s;
    s.set(ADDR(QStandardPaths, writableLocation), ADDR(ut_search_db_test, stub_writableLocation));
    QString databasePath1 = firstremovedbfile();

    QSignalSpy spy(sd, SIGNAL(searchContentMismatch(const QString &)));
    QSignalSpy spy1(sd, SIGNAL(searchContentResult(const QString &,
                                                   const QStringList &,
                                                   const QStringList &,
                                                   const QStringList &)));

    QString system = "professional";
    QString app_name = "dde";
    QString lang = "zh_CN";

    QStringList anchors {"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
    QStringList anchorInitialList {"gs", "syrm", "qhsrf", "ztl", "sxsz", "jbsz", "mrsz"};
    QStringList anchorSpellList {"gaishu", "shiyongrumen", "qiehuanshurufa", "zhuangtailan", "shuxingshezhi", "jibenshezhi", "morenshezhi"};
    QStringList anchorIdList {"h0", "h1", "h2", "h3", "h4", "h5", "h6"};
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
    sd->addSearchEntry(app_name, "en_US", anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
    sd->addSearchEntry(app_name, "zh_TW", anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
    sd->addSearchEntry(app_name, "zh_HK", anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
    sd->addSearchEntry(app_name, "bo_CN", anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);

    sd->handleSearchContent("&");
    ASSERT_EQ(spy.count(), 1);

    s.set(ADDR(QLocale, name), stub_LocalNameug_zh_CN);
    sd->strlistApp << "dde";
    sd->handleSearchContent("输入法配置");
    ASSERT_EQ(spy1.count(), 1);

    s.set((bool (QSqlQuery::*)(const QString &))ADDR(QSqlQuery, exec), stub_openFalse);
    sd->handleSearchContent("输入法配置");
    ASSERT_EQ(spy.count(), 2);
}

TEST_F(ut_search_db_test, insertFilesTimeEntry)
{
    stt = new Stub;
    stt->set(ADDR(QStandardPaths, writableLocation), ADDR(ut_search_db_test, stub_writableLocation));

    QString databasePath1 = firstremovedbfile();

    QStringList listMdPath;
    QStringList listDataTime;
    listMdPath << "aaaa"
               << "A";
    listDataTime << "0011"
                 << "0022";

    sd->initDb();
    QFile file(databasePath1);

    if (file.exists()) {
        qWarning() << "db file exisits";
    }

    sd->initTimeTable();

    sd->insertFilesTimeEntry(listMdPath, listDataTime);

    QString sql = "select count(1) from filetime";

    //创建数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString(QUuid::WithoutBraces));
    db.setDatabaseName(databasePath1);
    db.open();

    QSqlQuery query(db);
    int icount = 0;
    query.exec(sql);
    if (query.first()) {
        icount = query.value(0).toInt();
        ASSERT_EQ(icount, 2);
    }

    query.exec("delete from filetime");

    stt->set((bool (QSqlQuery::*)())ADDR(QSqlQuery, execBatch), stub_openFalse);
    sd->insertFilesTimeEntry(listMdPath, listDataTime);
    stt->reset((bool (QSqlQuery::*)())ADDR(QSqlQuery, execBatch));
    query.exec(sql);
    if (query.first()) {
        icount = query.value(0).toInt();
        ASSERT_EQ(icount, 0);
    }
    db.close();

    stt->set((bool (QSqlDatabase::*)())ADDR(QSqlDatabase, transaction), stub_openFalse);
    sd->insertFilesTimeEntry(listMdPath, listDataTime);
    stt->reset((bool (QSqlDatabase::*)())ADDR(QSqlDatabase, transaction));
    db.open();
    query.exec(sql);
    if (query.first()) {
        icount = query.value(0).toInt();
        ASSERT_EQ(icount, 0);
    }
    db.close();

    stt->set((bool (QSqlDatabase::*)())ADDR(QSqlDatabase, transaction), ADDR(ut_search_db_test, stub_transactionture));
    sd->insertFilesTimeEntry(listMdPath, listDataTime);
    stt->reset((bool (QSqlDatabase::*)())ADDR(QSqlDatabase, transaction));
    db.open();
    query.exec(sql);
    if (query.first()) {
        icount = query.value(0).toInt();
        ASSERT_EQ(icount, 0);
    }
    db.close();
    m_count = 0;
    stt->set((bool (QSqlQuery::*)())ADDR(QSqlQuery, prepare), stub_exec);
    sd->insertFilesTimeEntry(listMdPath, listDataTime);
    stt->reset((bool (QSqlQuery::*)())ADDR(QSqlQuery, prepare));
    db.open();
    sql = "select count(1) from filetime";
    query.exec(sql);
    if (query.first()) {
        icount = query.value(0).toInt();
        ASSERT_EQ(icount, 0);
    }
    db.close();
    delete stt;
    stt = nullptr;
}

TEST_F(ut_search_db_test, deleteFilesTimeEntry)
{
    Stub s;
    s.set(ADDR(QStandardPaths, writableLocation), ADDR(ut_search_db_test, stub_writableLocation));
    QString databasePath1 = firstremovedbfile();

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

    QString sql = "select count(1) from filetime";

    //创建数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString(QUuid::WithoutBraces));
    db.setDatabaseName(databasePath1);
    db.open();

    QSqlQuery query(db);

    query.exec(sql);
    int icount = 0;
    if (query.first()) {
        icount = query.value(0).toInt();
        ASSERT_EQ(icount, 0);
    }
    db.close();

    sd->insertFilesTimeEntry(listMdPath, listTime);
    s.set((bool (QSqlQuery::*)())ADDR(QSqlQuery, execBatch), stub_openFalse);
    sd->deleteFilesTimeEntry(listMdPath);
    s.reset((bool (QSqlQuery::*)())ADDR(QSqlQuery, execBatch));
    db.open();
    query.exec(sql);
    if (query.first()) {
        icount = query.value(0).toInt();
        ASSERT_EQ(icount, 2);
    }
    db.close();

    m_count = 0;
    s.set((bool (QSqlQuery::*)())ADDR(QSqlQuery, prepare), stub_exec);
    sd->deleteFilesTimeEntry(listMdPath);

    db.open();
    query.exec(sql);
    if (query.first()) {
        icount = query.value(0).toInt();
        ASSERT_EQ(icount, 2);
    }
    db.close();
}

TEST_F(ut_search_db_test, selectAllFileTime)
{
    Stub s;
    s.set(ADDR(QStandardPaths, writableLocation), ADDR(ut_search_db_test, stub_writableLocation));
    QString databasePath1 = firstremovedbfile();

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
    ASSERT_EQ(map["aaaa"], "0011");
    ASSERT_EQ(map["A"], "0022");
}

TEST_F(ut_search_db_test, getAllApp)
{
    QStringList list = Utils::getSystemManualList();
    sd->getAllApp();
    ASSERT_EQ(list, sd->strlistApp);
}
