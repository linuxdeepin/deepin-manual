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
#define private public
#include "base/utils.h"
#undef private
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

/*
TEST_F(ut_utils_test, getTime)
{
    struct timeval tp;
    gettimeofday(&tp, nullptr);
    ASSERT_EQ(m_utils->getTime().tv_sec, tp.tv_sec);
}
TEST_F(ut_utils_test, showDiffTime)
{
    struct timeval tp;
    gettimeofday(&tp, nullptr);
    //sleep(1);
    struct timeval tmp = m_utils->showDiffTime(tp);

    double timeuse =
        (1000000 * (tmp.tv_sec - tp.tv_sec) + tmp.tv_usec - tp.tv_usec) / 1000000.0;

    qDebug() << "test----------" <<  timeuse;
    ASSERT_EQ(m_utils->showDiffTime(tp).tv_sec, tmp.tv_sec);
}


TEST_F(ut_utils_test, getQssContent)
{
    QString strMANUAL_DIR = DMAN_MANUAL_DIR;
    strMANUAL_DIR.replace("manual-assets", "src/web/sass/nav.scss");

    qDebug() << "test--dir.size()-->" << m_utils->getQssContent(strMANUAL_DIR).size();
    ASSERT_NE(m_utils->getQssContent(strMANUAL_DIR).size(), 0);
}
*/

TEST_F(ut_utils_test, renderSVG)
{
    QPixmap pix = m_utils->renderSVG(QString(kImageDarkSearchIcon), QSize(20, 20));
    qDebug() << pix;
    ASSERT_EQ(pix.width(), 20);
    ASSERT_EQ(pix.height(), 20);
}

TEST_F(ut_utils_test, renderSVG2)
{
    QString imgPath = kImageDarkSearchIcon;
    imgPath += "xx";
    QPixmap pix = m_utils->renderSVG(imgPath, QSize(20, 20));
//    ASSERT_EQ(pix.width(), 20);
//    ASSERT_EQ(pix.height(), 20);
}

/*
TEST_F(ut_utils_test, loadFontFamilyByType)
{
    m_utils->loadFontFamilyByType(m_utils->FontType:: SourceHanSansMedium);
}

TEST_F(ut_utils_test, loadFontFamilyByType2)
{
    QString str = m_utils->loadFontFamilyByType(m_utils->FontType:: SourceHanSansNormal);
    ASSERT_EQ(str, "");
}

TEST_F(ut_utils_test, loadFontFamilyByType3)
{
    m_utils->loadFontFamilyByType(m_utils->FontType:: DefautFont);
}

TEST_F(ut_utils_test, loadFontBySizeAndWeight)
{
    QFont font = m_utils->loadFontBySizeAndWeight("arial", 20, 20);
    ASSERT_EQ(font.family(), "arial");
}

TEST_F(ut_utils_test, fromSpecialEncoding)
{
    ASSERT_EQ(m_utils->fromSpecialEncoding("帮助"), "帮助");
}

TEST_F(ut_utils_test, fromSpecialEncoding2)
{
    ASSERT_EQ(m_utils->fromSpecialEncoding("1"), "1");
}
*/

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
    ASSERT_TRUE(m_utils->hasSelperSupport());
}

TEST_F(ut_utils_test, exapplicationHelperInstance)
{
    ExApplicationHelper *eh = ExApplicationHelper::instance();
}

TEST_F(ut_utils_test, standardPalette)
{
    ExApplicationHelper ex;
//    ExApplicationHelper::standardPalette(DGuiApplicationHelper::DarkType);
    ex.standardPalette(DGuiApplicationHelper::DarkType);
}

TEST_F(ut_utils_test, eventFilter)
{
    ExApplicationHelper ex;
    QEvent *e;
    ex.eventFilter(&ex, e);
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
