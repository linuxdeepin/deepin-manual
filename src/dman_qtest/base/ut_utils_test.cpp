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
#include "ut_utils_test.h"
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

TEST_F(ut_utils_test, suffixList)
{
    ASSERT_EQ(m_utils->suffixList(), "Font Files (*.ttf *.ttc *.otf)");
}

TEST_F(ut_utils_test, getSystemManualList)
{
    QStringList list = {"deepin-defender", "deepin-picker", "deepin-voice-note", "deepin-browser",
                        "dde-calendar", "dde-file-manager", "deepin-album", "deepin-app-store",
                        "deepin-compressor", "deepin-calculator", "deepin-deb-installer",
                        "deepin-editor", "deepin-draw", "deepin-devicemanager", "deepin-font-manager",
                        "deepin-image-viewer", "deepin-screen-recorder", "deepin-reader",
                        "deepin-log-viewer", "deepin-system-monitor", "deepin-terminal", "deepin-contacts",
                        "scaner", "uos-service-support", "deepin-boot-maker", "deepin-movie",
                        "dde-printer", "deepin-music", "dde"
                       };
    qDebug() << list.size();
    ASSERT_EQ(m_utils->getSystemManualList().size(), list.size());
}

TEST_F(ut_utils_test, getSystemManualDir)
{
    QString str = "/home/kevin_w/project/qtest/manual/manual-assets/professional";
    ASSERT_EQ(m_utils->getSystemManualDir(), str);
}

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
TEST_F(ut_utils_test, renderSVG)
{
    QPixmap pix = m_utils->renderSVG(QString(dman::kImageDarkSearchIcon), QSize(20, 20));
    ASSERT_EQ(pix.width(), 20);
    ASSERT_EQ(pix.height(), 20);
}
TEST_F(ut_utils_test, translateTitle)
{
    ASSERT_EQ(m_utils->translateTitle("accounts"), "账户设置");
}
TEST_F(ut_utils_test, hasSelperSupport)
{
    ASSERT_TRUE(m_utils->hasSelperSupport());
}
