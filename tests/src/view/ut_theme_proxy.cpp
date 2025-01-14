// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_theme_proxy.h"
#include "view/theme_proxy.h"

#include <DGuiApplicationHelper>

#include <QSignalSpy>

ut_theme_proxy_test::ut_theme_proxy_test()
{

}

TEST_F(ut_theme_proxy_test, getTheme)
{
    ThemeProxy tp;

    DTK_GUI_NAMESPACE::DGuiApplicationHelper::instance()->setPaletteType(DTK_GUI_NAMESPACE::DGuiApplicationHelper::LightType);
    ASSERT_EQ(tp.getTheme(), "LightType");

    DTK_GUI_NAMESPACE::DGuiApplicationHelper::instance()->setPaletteType(DTK_GUI_NAMESPACE::DGuiApplicationHelper::DarkType);
    ASSERT_EQ(tp.getTheme(), "DarkType");


}

TEST_F(ut_theme_proxy_test, slot_ThemeChange)
{
    ThemeProxy tp;
    QSignalSpy spy(&tp, SIGNAL(themeChange(const QString &)));

    tp.slot_ThemeChange();
    qWarning() << spy.count();
    QList<QVariant> argu = spy.takeFirst();
    ASSERT_EQ(argu.at(0).toString(), tp.getTheme());
}

