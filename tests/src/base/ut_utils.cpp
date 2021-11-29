/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     wangmingliang <wangmingliang@uniontech.com>
* Maintainer: wangmingliang <wanmgmingliang@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ut_utils.h"
#include "base/utils.h"
#include "resources/themes/images.h"
#include "src/third-party/stub/stub.h"

ut_utils_test::ut_utils_test()
{

}

bool ut_utils_test::stub_exists() const
{
    return true;
}

TEST_F(ut_utils_test, getSystemManualList)
{
    Utils *m_utils = new Utils;
    ASSERT_TRUE(m_utils != nullptr);
    /*QStringList list = {"deepin-defender", "deepin-picker", "deepin-voice-note", "deepin-browser",
                        "dde-calendar", "dde-file-manager", "deepin-album", "deepin-app-store",
                        "deepin-compressor", "deepin-calculator", "deepin-deb-installer",
                        "deepin-editor", "deepin-draw", "deepin-devicemanager", "deepin-font-manager",
                        "deepin-image-viewer", "deepin-screen-recorder", "deepin-reader",
                        "deepin-log-viewer", "deepin-system-monitor", "deepin-terminal", "deepin-contacts",
                        "scaner", "uos-service-support", "deepin-boot-maker", "deepin-movie",
                        "dde-printer", "deepin-music", "dde"
                       };
    qDebug() << list.size();
    */
    //ASSERT_EQ(m_utils->getSystemManualList().size(), list.size());
    QString olddir = DMAN_MANUAL_DIR;
    Dtk::Core::DSysInfo::DeepinType currtype = Dtk::Core::DSysInfo::deepinType();
    Stub s;
    s.set(ADDR(Dtk::Core::DSysInfo, deepinType), stub_deepinTypeServer);
    ASSERT_TRUE(m_utils->getSystemManualList().size() > 0);

    s.reset(ADDR(Dtk::Core::DSysInfo, deepinType));
    s.set(ADDR(Dtk::Core::DSysInfo, deepinType), stub_deepinTypeDeepinPersonal);
    ASSERT_TRUE(m_utils->getSystemManualList().size() > 0);

    s.reset(ADDR(Dtk::Core::DSysInfo, deepinType));

    s.set(ADDR(Dtk::Core::DSysInfo, deepinType), stub_deepinTypeDeepinUnknownDeepin);
    s.set(ADDR(Dtk::Core::DSysInfo, isCommunityEdition), stub_deepinTypeDeepinCommunity);
    ASSERT_TRUE(m_utils->getSystemManualList().size() > 0);

    delete m_utils;
}

TEST_F(ut_utils_test, getSystemManualDir)
{
    Utils *m_utils = new Utils;
    QString str = DMAN_MANUAL_DIR;

    ASSERT_EQ(m_utils->getSystemManualDir(), str);
    delete m_utils;
}


TEST_F(ut_utils_test, mkMutiDir)
{
    Utils *m_utils = new Utils;
    QString strbasepath = "/tmp";
    QString strpath = strbasepath.append("/dirtest/12/13");
    QString rmstrpath = strbasepath.append("/dirtest");
    QProcess pro;
    QString command = "rm -rf "+rmstrpath;
    pro.start(command);
    pro.waitForFinished();

    QString rmpath = m_utils->mkMutiDir(rmstrpath);
    QDir mkdir(rmstrpath);
    ASSERT_TRUE(mkdir.exists());
    ASSERT_EQ(rmpath, rmstrpath);

    rmpath = m_utils->mkMutiDir(strpath);
    ASSERT_EQ(rmpath, strpath);

    delete m_utils;
}

TEST_F(ut_utils_test, renderSVG)
{
    Utils *m_utils = new Utils;
    QPixmap pix = m_utils->renderSVG(QString(kImageDarkSearchIcon), QSize(20, 20));
    qDebug() << pix;
    ASSERT_EQ(pix.width(), 20);
    ASSERT_EQ(pix.height(), 20);

    delete m_utils;
}


TEST_F(ut_utils_test, sortAppList)
{
    AppInfo info;
    info.key = "key1";
    info.installed_time = 0002;
    info.desktop = "desktop";
    info.name = "name";
    info.category_id = 001;
    info.icon_key = "icon";

    AppInfo info1;
    info1.key = "key2";
    info1.installed_time = 0002;
    info1.desktop = "desktop";
    info1.name = "aame";
    info1.category_id = 001;
    info1.icon_key = "icon";

    AppInfo info2;
    info2.key = "key3";
    info2.installed_time = 0003;
    info2.desktop = "desktop";
    info2.name = "bame";
    info2.category_id = 001;
    info2.icon_key = "icon";

    AppInfo info3;
    info3.key = "key4";
    info3.installed_time = 0006;
    info3.desktop = "desktop";
    info3.name = "qame";
    info3.category_id = 001;
    info3.icon_key = "icon";

    QMultiMap<qlonglong, AppInfo> appmultma;
    appmultma.insert(1, info);
    appmultma.insert(4, info1);
    appmultma.insert(2, info2);
    appmultma.insert(4, info3);
    Utils *m_utils = new Utils;
    QList<AppInfo> resList = m_utils->sortAppList(appmultma);
    ASSERT_EQ(resList[0].key, "key1");
    ASSERT_EQ(resList[1].key, "key3");
    ASSERT_EQ(resList[2].key, "key2");
    ASSERT_EQ(resList[3].key, "key4");

    QMultiMap<qlonglong, AppInfo> appmultma2;
    appmultma2.insert(1, info);
    appmultma2.insert(4, info1);
    appmultma2.insert(3, info2);
    appmultma2.insert(2, info3);
    QList<AppInfo> resList2 = m_utils->sortAppList(appmultma2);
    ASSERT_EQ(resList2[0].key, "key1");
    ASSERT_EQ(resList2[1].key, "key4");
    ASSERT_EQ(resList2[2].key, "key3");
    ASSERT_EQ(resList2[3].key, "key2");

    delete m_utils;
}

TEST_F(ut_utils_test, renderSVG2)
{
    QString imgPath = kImageDarkSearchIcon;
    imgPath += "xx";
    Utils *m_utils = new Utils;
    QPixmap pix = m_utils->renderSVG(imgPath, QSize(20, 20));
    delete m_utils;
    ASSERT_TRUE(pix.isNull());
}



TEST_F(ut_utils_test, translateTitle)
{
    Utils *m_utils = new Utils;

    QString str = m_utils->translateTitle("controlcenter");
    if (QLocale::system().name() == "zh_CN")
        ASSERT_EQ(str, "控制中心");
    else
        ASSERT_EQ(str, "Control Center");

    Stub s;
    s.set(ADDR(QLocale, name), stub_LocalNameug_CN);
    QString transsStr = m_utils->translateTitle("accounts");
    ASSERT_EQ(transsStr, "帐户设置");
    s.reset(ADDR(QLocale, name));

    s.set(ADDR(QLocale, name), stub_LocalNamezh_HK);
    transsStr = m_utils->translateTitle("accounts");
    ASSERT_EQ(transsStr, "賬號設置");
    s.reset(ADDR(QLocale, name));

    s.set(ADDR(QLocale, name), stub_LocalNamezh_TW);
    transsStr = m_utils->translateTitle("accounts");
    ASSERT_EQ(transsStr, "帳戶設定");
    s.reset(ADDR(QLocale, name));

    s.set(ADDR(QLocale, name), stub_LocalNameug_EU);
    transsStr = m_utils->translateTitle("accounts");
    ASSERT_EQ(transsStr, "Accounts");
    s.reset(ADDR(QLocale, name));

    delete m_utils;
}

int usercnt = 0;

bool ut_utils_test::stub_isValid()
{
    return false;
}

QByteArray longsoon()
{
    return "Loongson";
}

struct ReplyStruct {
    QString m_desktop;
    QString m_name;
    QString m_key;
    QString m_iconKey;

    qint64 m_categoryId;
    qint64 m_installedTime;
};


TEST_F(ut_utils_test, launcherInterface)
{
    Utils *m_utils = new Utils;
    QList<AppInfo> applist = m_utils->launcherInterface();
    ASSERT_TRUE(applist.size() >= 0);

    Stub s;
    s.set(ADDR(QDBusInterface, isValid), stub_isValid);
    s.set(ADDR(QDBusReply<QList<ReplyStruct>>, isValid), stub_isValid);
    QList<AppInfo> applist2 = m_utils->launcherInterface();
    s.reset(ADDR(QDBusInterface, isValid));
    ASSERT_TRUE(applist2.size() == 0);

    QList<AppInfo> applist3 = m_utils->launcherInterface();
    ASSERT_TRUE(applist2.size() == 0);

    delete m_utils;
}

QStringList manaulapplist()
{
    QStringList applist;
    applist << "uos-service-support";
    return applist;
}

TEST_F(ut_utils_test, hasSelperSupport)
{
//    ASSERT_TRUE(m_utils->hasSelperSupport());
    Utils *m_utils = new Utils;
    Stub s;
    s.set(ADDR(Utils, getSystemManualList), manaulapplist);
    int nType = Dtk::Core::DSysInfo::deepinType();
    if (Dtk::Core::DSysInfo::DeepinProfessional == nType)
        ASSERT_TRUE(m_utils->hasSelperSupport());
    else
        ASSERT_FALSE(m_utils->hasSelperSupport());

    delete m_utils;
}


TEST_F(ut_utils_test, systemToOmit)
{
    Utils *m_utils = new Utils;
    QStringList omitlist = m_utils->systemToOmit(Dtk::Core::DSysInfo::UosProfessional);
    ASSERT_TRUE( omitlist.contains("p"));

    omitlist = m_utils->systemToOmit(Dtk::Core::DSysInfo::UosHome);
    ASSERT_TRUE( omitlist.contains("h"));

    omitlist = m_utils->systemToOmit(Dtk::Core::DSysInfo::UosCommunity);
    ASSERT_TRUE( omitlist.contains("d"));

    omitlist = m_utils->systemToOmit(Dtk::Core::DSysInfo::UosEnterprise);
    ASSERT_TRUE( omitlist.contains("e") && omitlist.contains("s")) ;


    omitlist = m_utils->systemToOmit(Dtk::Core::DSysInfo::UosEnterpriseC);
    ASSERT_TRUE( omitlist.contains("i") && omitlist.contains("s")) ;

    omitlist = m_utils->systemToOmit(Dtk::Core::DSysInfo::UosEuler);
    ASSERT_TRUE( omitlist.contains("eu") && omitlist.contains("s")) ;
    delete m_utils;
}

TEST_F(ut_utils_test, exapplicationHelperInstance)
{
    ExApplicationHelper *p = ExApplicationHelper::instance();

    ASSERT_FALSE(p != nullptr);
}

TEST_F(ut_utils_test, judgeLongson)
{
    QString cpuModeName;
    QProcess process;
    //获取CPU型号
    process.start("cat /proc/cpuinfo");

    if (process.waitForFinished()) {
        QString result = process.readAllStandardOutput();

        if (result.contains("Loongson")) {
            qWarning() << "cpu mode name is loongson";
            cpuModeName = "Loongson";
        } else {
            cpuModeName = "other";
        }
    }

    Utils *m_utils = new Utils;
    bool ret = m_utils->judgeLoongson();
    ASSERT_EQ(ret, cpuModeName.contains("Loongson"));

    Stub s;
    s.set(ADDR(QProcess, readAllStandardOutput), longsoon);
    m_utils->cpuModeName = "";
    ret = m_utils->judgeLoongson();
    ASSERT_EQ(ret, true);
    m_utils->deleteLater();
}

TEST_F(ut_utils_test, activeWindow)
{
    Utils *m_utils = new Utils;
    ASSERT_FALSE(m_utils->activeWindow(123));
    delete m_utils;
}

TEST_F(ut_utils_test, regexp_label)
{
    Utils *m_utils = new Utils;
    QFile file("/usr/share/glib-2.0/schemas/com.deepin.dde.appearance.gschema.xml");


    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QString strContent(file.readAll());
        strContent = m_utils->regexp_label(strContent, "(icon-theme\">\n)(.*)?(['</default>])");
        qWarning() << strContent;
        ASSERT_TRUE(strContent.contains("icon-theme\">"));
        ASSERT_TRUE(strContent.contains("</default>"));
    }
    delete m_utils;
}

TEST_F(ut_utils_test, standardPalette)
{
    ExApplicationHelper ex;
    DPalette pa = ex.standardPalette(DGuiApplicationHelper::DarkType);
    ASSERT_EQ(pa.color(DPalette::TextTitle), QColor(0xC0, 0xC6, 0xD4));
    pa = ex.standardPalette(DGuiApplicationHelper::LightType);
    ASSERT_EQ(pa.color(DPalette::TextTitle), QColor(0x00, 0x1A, 0x2E));
}

TEST_F(ut_utils_test, eventFilter)
{
    ExApplicationHelper ex;
    QEvent *e;
    ASSERT_FALSE(ex.eventFilter(&ex, e));
}

TEST_F(ut_utils_test, palette)
{
    QWidget *w = new QWidget;
    DPalette p;
    ExApplicationHelper ex;
    p = ex.palette(w, p);
    if (DGuiApplicationHelper::toColorType(w->palette()) == DGuiApplicationHelper::DarkType) {
        ASSERT_EQ(p.color(DPalette::TextTitle), QColor(0xC0, 0xC6, 0xD4));
    } else {
        ASSERT_EQ(p.color(DPalette::TextTitle), QColor(0x00, 0x1A, 0x2E));
    }

    delete w;
}

TEST_F(ut_utils_test, setPalette)
{
    QWidget *w = new QWidget;
    DPalette p;
    p.setColor(DPalette::WindowText, QColor(0xC0, 0xC6, 0xD4));
    ;
    ExApplicationHelper ex;
    ex.setPalette(w, p);
    DPalette p1 = w->palette();
    ASSERT_EQ(p1.color(DPalette::WindowText), QColor(0xC0, 0xC6, 0xD4));
    w->deleteLater();
}

TEST_F(ut_utils_test, ExApplicationHelper)
{
    ExApplicationHelper *ex = new ExApplicationHelper;
    ex->deleteLater();
}
