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
#include "ut_search_button_test.h"

#define private public
#define protected public
#include "view/widget/search_button.h"

#undef private
#undef protected

#include "resources/themes/images.h"
#include "base/utils.h"

#include <QGuiApplication>
#include <QEvent>


namespace dman {

ut_search_button_test::ut_search_button_test()
{

}
TEST_F(ut_search_button_test, updateColor)
{
    SearchButton sb;
    QColor co(255, 0, 0);
    sb.updateColor(co);
    ASSERT_EQ(sb.myColor, co);
}

TEST_F(ut_search_button_test, setText)
{
    SearchButton sb;
    sb.setText("abc");
    ASSERT_EQ(sb.m_textLabel->text(), "abc");
}

TEST_F(ut_search_button_test, isChecked)
{
    SearchButton sb;
    sb.m_bHover = false;
    sb.isChecked();
    ASSERT_FALSE(sb.m_bHover);
}

TEST_F(ut_search_button_test, leaveFocus)
{
    DTK_GUI_NAMESPACE::DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::DarkType);
    SearchButton sb;
    sb.leaveFocus();
    ASSERT_FALSE(sb.m_bHover);
    QPixmap iconPm = Utils::renderSVG(QString(kImageDarkSearchIcon), QSize(20, 20));
    ASSERT_EQ(sb.iconBtn->icon().pixmap(QSize(20, 20)), iconPm);
}

TEST_F(ut_search_button_test, leaveFocus2)
{
    SearchButton sb;
    DTK_GUI_NAMESPACE::DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::LightType);
    sb.leaveFocus();
    ASSERT_FALSE(sb.m_bHover);
    QPixmap iconPm1 = Utils::renderSVG(QString(kImageLightSearchIcon), QSize(20, 20));
    ASSERT_EQ(sb.iconBtn->icon().pixmap(QSize(20, 20)), iconPm1);

}

TEST_F(ut_search_button_test, paintEvent)
{
    SearchButton sb;
    sb.m_bHover = true;
    QPaintEvent *event;
    sb.paintEvent(event);
    QPixmap iconPm = Utils::renderSVG(QString(kImageWhiteSearchIcon), QSize(20, 20));
    ASSERT_EQ(sb.iconBtn->icon().pixmap(QSize(20, 20)), iconPm);
}

TEST_F(ut_search_button_test, mouseReleaseEvent)
{
    SearchButton *sb = new SearchButton;
    QTest::mouseClick(sb, Qt::LeftButton);
    ASSERT_FALSE(sb->isVisible());

}
TEST_F(ut_search_button_test, enterEvent)
{
    SearchButton sb;
    QEvent *e;
    sb.m_bHover = false;
    sb.enterEvent(e);
    ASSERT_TRUE(sb.m_bHover);
}

TEST_F(ut_search_button_test, leaveEvent)
{
    QEvent *e;
    DTK_GUI_NAMESPACE::DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::DarkType);
    SearchButton sb;
    sb.leaveEvent(e);
    ASSERT_FALSE(sb.m_bHover);
    QPixmap iconPm = Utils::renderSVG(QString(kImageDarkSearchIcon), QSize(20, 20));
    ASSERT_EQ(sb.iconBtn->icon().pixmap(QSize(20, 20)), iconPm);

}

TEST_F(ut_search_button_test, leaveEvent2)
{
    QEvent *e;
    SearchButton sb;
    DTK_GUI_NAMESPACE::DGuiApplicationHelper::instance()->setThemeType(DGuiApplicationHelper::LightType);
    sb.leaveEvent(e);
    ASSERT_FALSE(sb.m_bHover);
    QPixmap iconPm1 = Utils::renderSVG(QString(kImageLightSearchIcon), QSize(20, 20));
    ASSERT_EQ(sb.iconBtn->icon().pixmap(QSize(20, 20)), iconPm1);

}

}
