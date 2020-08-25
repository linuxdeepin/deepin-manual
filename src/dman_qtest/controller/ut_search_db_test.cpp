#include "ut_search_db_test.h"

#define private public
#include "controller/search_db.h"
#undef private
namespace dman {

ut_search_db_test::ut_search_db_test()
{

}

TEST_F(ut_search_db_test, initSearchTable)
{

    QString strCreateDbPath = DMAN_SEARCH_CREATE_DB_PATH;
    QDir dir(strCreateDbPath);
    if (!dir.exists()) {
        dir.mkpath(strCreateDbPath);
    }
    strCreateDbPath += "/search.db";
    SearchDb sd;
    //sd.initDb(strCreateDbPath);
    sd.initSearchTable();
}

TEST_F(ut_search_db_test, initSearchTable2)
{

    QString strCreateDbPath = DMAN_SEARCH_CREATE_DB_PATH;
    QDir dir(strCreateDbPath);
    if (!dir.exists()) {
        dir.mkpath(strCreateDbPath);
    }
    strCreateDbPath += "/search.db";
    SearchDb sd;
    sd.initDb(strCreateDbPath);
    //匿名命名空间 在其它文件中无法使用。。。

    sd.initSearchTable();
}

TEST_F(ut_search_db_test, addSearchEntry)
{
    QString system = "professional";
    QString app_name = "chineseime";
    QString lang = "zh_CN";

    QStringList anchors{"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
    QStringList anchorInitialList{"gs", "syrm", "qhsrf", "ztl", "sxsz", "jbsz", "mrsz"};
    QStringList anchorSpellList{"gaishu", "shiyongrumen", "qiehuanshurufa", "zhuangtailan", "shuxingshezhi", "jibenshezhi", "morenshezhi"};
    QStringList anchorIdList{"h0", "h0", "h0", "h0", "h0", "h0", "h0"};
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
    if (!dir.exists()) {
        dir.mkpath(strCreateDbPath);
    }
    strCreateDbPath += "/search.db";
    sd.initDb(strCreateDbPath);
    sd.initSearchTable();
    sd.addSearchEntry(system, app_name, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
}

TEST_F(ut_search_db_test, addSearchEntry2)
{
    QString system = "professional";
    QString app_name = "chineseime";
    QString lang = "zh_CN";

    QStringList anchors{"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
    QStringList anchorInitialList{"gs", "syrm", "qhsrf", "ztl", "sxsz", "jbsz", "mrsz"};
    QStringList anchorSpellList{"gaishu", "shiyongrumen", "qiehuanshurufa", "zhuangtailan", "shuxingshezhi", "jibenshezhi", "morenshezhi"};
    QStringList anchorIdList{"h0", "h0", "h0", "h0", "h0", "h0"}; //六项
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
    if (!dir.exists()) {
        dir.mkpath(strCreateDbPath);
    }
    strCreateDbPath += "/search.db";
    sd.initDb(strCreateDbPath);
    sd.initSearchTable();
    sd.addSearchEntry(system, app_name, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);

//    QProcess p;
//    p.start("cp /usr/share/deepin-manual/manual-assets/professional/search.db "
//            "/home/kevin_w/project/qtest/build-manual-unknown-Debug/src/dbPath/");
//    p.close();
}

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



TEST_F(ut_search_db_test, handleSearchAchor)
{
    QString system = "professional";
    QString app_name = "chineseime";
    QString lang = "zh_CN";

    QStringList anchors{"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
    QStringList anchorInitialList{"gs", "syrm", "qhsrf", "ztl", "sxsz", "jbsz", "mrsz"};
    QStringList anchorSpellList{"gaishu", "shiyongrumen", "qiehuanshurufa", "zhuangtailan", "shuxingshezhi", "jibenshezhi", "morenshezhi"};
    QStringList anchorIdList{"h0", "h0", "h0", "h0", "h0", "h0", "h0"};
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
    if (!dir.exists()) {
        dir.mkpath(strCreateDbPath);
    }
    strCreateDbPath += "/search.db";
    sd.initDb(strCreateDbPath);
    sd.initSearchTable();
    sd.addSearchEntry(system, app_name, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
    sd.handleSearchAnchor("概述");
}

TEST_F(ut_search_db_test, insertHighlight)
{
    //全局函数，无法测试
    //QString insertHighlight(QString srcString, QString keyword)
}

TEST_F(ut_search_db_test, highlightKeyword)
{
    QString srcString = "系统会预装中文输入法";
    QString srcKeyword = "中文";
    QString value = "系统会预装<span class='highlight'>中文</span>输入法";
    SearchDb sd;
    QString tmp = sd.highlightKeyword(srcString, srcKeyword);
    ASSERT_EQ(tmp, value);
}

TEST_F(ut_search_db_test, sortSearchList)
{
    QString appName = "chineseime";
    QStringList anchors{"概述", "使用入门", "切换输入法", "状态栏", "属性设置", "基本设置", "默认设置"};
    QStringList anchorIdList{"h0", "h0", "h0", "h0", "h0", "h0", "h0"};
    QStringList contents{"00系统会预装中文输入法，单击右下角托盘上的",
                         "01进入输入法配置页面。单击 输入法，选择中文输入法",
                         "02托盘中输入法图标变为<img，则表示切换成功。",
                         "03系统会预装中文输入法，单击右下角托盘上的",
                         "04seime/zh_CN/icon/onboard.svg",
                         "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法图标变为<img src=",
                         "06/usr/share/deepin-manual/manual-assets/professional/chineseime/zh_CN/icon/inputer.svg"
                        };
//    sortSearchList(const QString &appName, const QStringList &anchors, const QStringList &anchorIds,
//                   const QStringList &contents, bool bIsTitleHigh

    SearchDb sd;
    sd.sortSearchList(appName, anchors, anchorIdList, contents, true);
}

TEST_F(ut_search_db_test, onitHighlight)
{
    QString srcString = "01进入输入法配置页面。单击 输入法，选择中文输入法"
                        "02托盘中输入法图g，则表示切换成功。"
                        "03系统会预装中文输入法，单击右下角托盘上的"
                        "04seime/zh_CN/icon/onboard.svg"
                        "05>进入输入法配置页面。单击 输入法，选择中文输入法，托盘中输入法"
                        "选择中文输入法，托盘中输入法图标变为<img src=\"/usr/share/deepin-manual/manual-assets/professional/chineseime/zh_CN/icon/inputer.svg\" >，则表"
                        ;
    QString srcKeyword = "图标";
    QString value = "系统会预装<span class='highlight'>中文</span>输入法";
    SearchDb sd;
    sd.omitHighlight(srcString, srcKeyword);
}


}
