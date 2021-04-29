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
#include <stdlib.h>

ut_utils_test::ut_utils_test()
{

}


TEST_F(ut_utils_test, getSystemManualList)
{
    Utils *m_utils = new Utils;
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
    Stub s;
    s.set(ADDR(Dtk::Core::DSysInfo, deepinType), stub_deepinType);
    m_utils->getSystemManualList();
    delete m_utils;
}


TEST_F(ut_utils_test, getSystemManualList2)
{
    Utils *m_utils = new Utils;
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
    Stub s;
    s.set(ADDR(Dtk::Core::DSysInfo, deepinType), stub_deepinTypeDeepinPersonal);
    m_utils->getSystemManualList();
    delete m_utils;
}

TEST_F(ut_utils_test, getSystemManualList3)
{
    Utils *m_utils = new Utils;
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
    Stub s;
    s.set(ADDR(Dtk::Core::DSysInfo, deepinType), stub_deepinTypeDeepinUnknownDeepin);
    s.set(ADDR(Dtk::Core::DSysInfo, isCommunityEdition), stub_deepinTypeDeepinCommunity);
    m_utils->getSystemManualList();
    delete m_utils;
}

TEST_F(ut_utils_test, getSystemManualDir)
{
    Utils *m_utils = new Utils;
    QString str = DMAN_MANUAL_DIR;

    ASSERT_EQ(m_utils->getSystemManualDir(), str);
    delete m_utils;
}

bool ut_utils_test::stub_exists() const
{
    return true;
}

TEST_F(ut_utils_test, mkMutiDir)
{
    Utils *m_utils = new Utils;
    QString strbasepath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append("/.local/share/deepin/deepin-manual");
    QString strpath = strbasepath.append("/dirtest/12/13");
    QString rmstrpath = strbasepath.append("/dirtest");
    QProcess pro;
    QString command = "rm -rf "+rmstrpath;
    pro.start(command);
    pro.waitForFinished();
    m_utils->mkMutiDir(rmstrpath);

    m_utils->mkMutiDir(strpath);

    Stub st;
    st.set((bool (QDir::*)() const)ADDR(QDir, exists), ADDR(ut_utils_test, stub_exists));
    m_utils->mkMutiDir(strpath);



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
    info.key = "key";
    info.installed_time = 0002;
    info.desktop = "desktop";
    info.name = "name";
    info.category_id = 001;
    info.icon_key = "icon";

    QMultiMap<qlonglong, AppInfo> appmultma;
    appmultma.insert(1, info);
    appmultma.insert(3, info);
    appmultma.insert(2, info);
    appmultma.insert(4, info);
    Utils *m_utils = new Utils;
    m_utils->sortAppList(appmultma);
    delete m_utils;
}

TEST_F(ut_utils_test, renderSVG2)
{
    QString imgPath = kImageDarkSearchIcon;
    imgPath += "xx";
    Utils *m_utils = new Utils;
    QPixmap pix = m_utils->renderSVG(imgPath, QSize(20, 20));
    delete m_utils;
}



TEST_F(ut_utils_test, translateTitle)
{
    Utils *m_utils = new Utils;
    Stub s;
    s.set(ADDR(QLocale, name), stub_LocalNameug_CN);
    m_utils->translateTitle("controlcenter");
    s.reset(ADDR(QLocale, name));

    s.set(ADDR(QLocale, name), stub_LocalNamezh_HK);
    m_utils->translateTitle("controlcenter");
    s.reset(ADDR(QLocale, name));

    s.set(ADDR(QLocale, name), stub_LocalNamezh_TW);
    m_utils->translateTitle("controlcenter");
    s.reset(ADDR(QLocale, name));

    delete m_utils;
}




TEST_F(ut_utils_test, translateTitle2)
{
    Utils *m_utils = new Utils;
    QString str = m_utils->translateTitle("controlcenter");
    if (QLocale::system().name() == "zh_CN")
        ASSERT_EQ(str, "控制中心");
    else
        ASSERT_EQ(str, "Control Center");
    delete m_utils;
}

int execcnt = 0;

bool ut_utils_test::stub_isValid()
{
    if(execcnt == 0)
    {
        execcnt++;
        return false;
    }
    return false;
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
    m_utils->launcherInterface();

    Stub s;
    s.set(ADDR(QDBusInterface, isValid), stub_isValid);
    s.set(ADDR(QDBusReply<QList<ReplyStruct>>, isValid), stub_isValid);
    m_utils->launcherInterface();

    delete m_utils;
}


TEST_F(ut_utils_test, hasSelperSupport)
{
//    ASSERT_TRUE(m_utils->hasSelperSupport());
    Utils *m_utils = new Utils;
    ASSERT_FALSE(m_utils->hasSelperSupport());
    delete m_utils;
}


TEST_F(ut_utils_test, systemToOmit)
{
//    ASSERT_TRUE(m_utils->hasSelperSupport());
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
    ExApplicationHelper::instance();
}

TEST_F(ut_utils_test, isMostPriority)
{
    QString mdPath;
    QString morePriorityPath;
    Utils *m_utils = new Utils;
    m_utils->isMostPriority(mdPath, morePriorityPath);
    delete m_utils;
}

TEST_F(ut_utils_test, judgeLongson)
{
    Stub s;
    s.set(ADDR(QProcess, readAllStandardOutput), stub_readAllStandardOutput);
    Utils *m_utils = new Utils;
    m_utils->judgeLoongson();
    delete m_utils;

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
    }
    delete m_utils;
}

TEST_F(ut_utils_test, standardPalette)
{
    ExApplicationHelper ex;
    ex.standardPalette(DGuiApplicationHelper::DarkType);
}

TEST_F(ut_utils_test, eventFilter)
{
    ExApplicationHelper ex;
    QEvent *e;
    ex.eventFilter(&ex, e);
}

TEST_F(ut_utils_test, palette)
{
    QWidget *w = new QWidget;
    DPalette p;
    ExApplicationHelper ex;
    ex.palette(w, p);
    delete w;
}

TEST_F(ut_utils_test, setPalette)
{
    QWidget *w = new QWidget;
    DPalette p;
    ExApplicationHelper ex;
    ex.setPalette(w, p);
}

TEST_F(ut_utils_test, ExApplicationHelper)
{
    ExApplicationHelper *ex = new ExApplicationHelper;
    ex->deleteLater();

}
