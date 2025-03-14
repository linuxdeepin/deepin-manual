// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_search_button.h"
#include "view/widget/search_button.h"
#include "resources/themes/images.h"
#include "base/utils.h"

#include <QGuiApplication>
#include <QEvent>


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
    DTK_GUI_NAMESPACE::DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::DarkType);
    SearchButton sb;
    sb.leaveFocus();
    ASSERT_FALSE(sb.m_bHover);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QPixmap iconPm = Utils::renderSVG(QString(kImageDarkSearchIcon), QSize(20, 20));
    ASSERT_EQ(sb.iconBtn->icon().pixmap(QSize(20, 20)), iconPm);
#endif
}

TEST_F(ut_search_button_test, leaveFocus2)
{
    SearchButton sb;
    DTK_GUI_NAMESPACE::DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::LightType);
    sb.leaveFocus();
    ASSERT_FALSE(sb.m_bHover);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QPixmap iconPm1 = Utils::renderSVG(QString(kImageLightSearchIcon), QSize(20, 20));
    ASSERT_EQ(sb.iconBtn->icon().pixmap(QSize(20, 20)), iconPm1);
#endif
}


TEST_F(ut_search_button_test, setChecked)
{
    SearchButton sb;
    sb.setChecked(false);
    ASSERT_FALSE(sb.m_bHover);
}


TEST_F(ut_search_button_test, onThemeChange)
{
    SearchButton sb;
    sb.onThemeChange(DGuiApplicationHelper::DarkType);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QPixmap iconPm = Utils::renderSVG(QString(kImageDarkSearchIcon), QSize(20, 20));
    ASSERT_EQ(sb.iconBtn->icon().pixmap(QSize(20, 20)), iconPm);
#endif
}


TEST_F(ut_search_button_test, onThemeChange2)
{
    SearchButton sb;
    sb.onThemeChange(DGuiApplicationHelper::LightType);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QPixmap iconPm = Utils::renderSVG(QString(kImageLightSearchIcon), QSize(20, 20));
    ASSERT_EQ(sb.iconBtn->icon().pixmap(QSize(20, 20)), iconPm);
#endif
}

TEST_F(ut_search_button_test, paintEvent)
{
    SearchButton sb;
    sb.m_bHover = true;
    QPaintEvent *event;
    sb.paintEvent(event);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QPixmap iconPm = Utils::renderSVG(QString(kImageWhiteSearchIcon), QSize(20, 20));
    ASSERT_EQ(sb.iconBtn->icon().pixmap(QSize(20, 20)), iconPm);
#endif
}

TEST_F(ut_search_button_test, paintEvent2)
{
    SearchButton sb;
    sb.m_bHover = false;
    QPaintEvent *event;

    sb.paintEvent(event);
    DPalette paLabel = ExApplicationHelper::instance()->palette(&sb);
    paLabel.setColor(DPalette::WindowText, paLabel.color(DPalette::Text));

    ASSERT_EQ(sb.m_textLabel->palette(), paLabel);
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
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QEvent *e;
#else
    QEnterEvent *e;
#endif
    sb.m_bHover = false;
    sb.enterEvent(e);
    ASSERT_TRUE(sb.m_bHover);
}

TEST_F(ut_search_button_test, leaveEvent)
{
    QEvent *e;
    DTK_GUI_NAMESPACE::DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::DarkType);
    SearchButton sb;
    sb.leaveEvent(e);
    ASSERT_FALSE(sb.m_bHover);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QPixmap iconPm = Utils::renderSVG(QString(kImageDarkSearchIcon), QSize(20, 20));
    ASSERT_EQ(sb.iconBtn->icon().pixmap(QSize(20, 20)), iconPm);
#endif
}

TEST_F(ut_search_button_test, leaveEvent2)
{
    QEvent *e;
    SearchButton sb;
    DTK_GUI_NAMESPACE::DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::LightType);
    sb.leaveEvent(e);
    ASSERT_FALSE(sb.m_bHover);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QPixmap iconPm1 = Utils::renderSVG(QString(kImageLightSearchIcon), QSize(20, 20));
    ASSERT_EQ(sb.iconBtn->icon().pixmap(QSize(20, 20)), iconPm1);
#endif
}
