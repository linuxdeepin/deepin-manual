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

ut_utils_test::ut_utils_test()
{

}

void ut_utils_test::SetUp()
{
    m_utils = new Utils;
}

void ut_utils_test::TearDown()
{
    delete m_utils;
}


TEST_F(ut_utils_test, getSystemManualList)
{
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
    m_utils->getSystemManualList();
}

TEST_F(ut_utils_test, getSystemManualDir)
{
    QString str = DMAN_MANUAL_DIR;
    ASSERT_EQ(m_utils->getSystemManualDir(), str);
}

TEST_F(ut_utils_test, mkMutiDir)
{
    QString strbasepath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append("/.local/share/deepin/deepin-manual");
    QString strpath = strbasepath.append("/dirtest/11/22/33/44");
    m_utils->mkMutiDir(strpath);
}

TEST_F(ut_utils_test, renderSVG)
{
    QPixmap pix = m_utils->renderSVG(QString(kImageDarkSearchIcon), QSize(20, 20));
    qDebug() << pix;
    ASSERT_EQ(pix.width(), 20);
    ASSERT_EQ(pix.height(), 20);
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

    m_utils->sortAppList(appmultma);
}

TEST_F(ut_utils_test, renderSVG2)
{
    QString imgPath = kImageDarkSearchIcon;
    imgPath += "xx";
    QPixmap pix = m_utils->renderSVG(imgPath, QSize(20, 20));
}



TEST_F(ut_utils_test, translateTitle)
{
    QString str = m_utils->translateTitle("controlcenter");
    if (QLocale::system().name() == "zh_CN")
        ASSERT_EQ(str, "控制中心");
    else
        ASSERT_EQ(str, "Control Center");

}

TEST_F(ut_utils_test, launcherInterface)
{

    m_utils->launcherInterface();
}


TEST_F(ut_utils_test, hasSelperSupport)
{
//    ASSERT_TRUE(m_utils->hasSelperSupport());
    ASSERT_FALSE(m_utils->hasSelperSupport());
}


TEST_F(ut_utils_test, systemToOmit)
{
//    ASSERT_TRUE(m_utils->hasSelperSupport());
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
}

TEST_F(ut_utils_test, exapplicationHelperInstance)
{
    ExApplicationHelper::instance();
}

TEST_F(ut_utils_test, activeWindow)
{
    ASSERT_FALSE(m_utils->activeWindow(123));
}

TEST_F(ut_utils_test, regexp_label)
{
    QFile file("/usr/share/glib-2.0/schemas/com.deepin.dde.appearance.gschema.xml");
    if (file.exists() && file.open(QIODevice::ReadWrite)) {
        QString strContent(file.readAll());
        strContent = m_utils->regexp_label(strContent, "(icon-theme\">\n)(.*)?(['</default>])");
    }
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
