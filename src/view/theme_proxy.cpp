// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "view/theme_proxy.h"
#include "base/ddlog.h"

#include <DGuiApplicationHelper>

DGUI_USE_NAMESPACE

ThemeProxy::ThemeProxy(QObject *parent)
    : QObject(parent)
{
    qCDebug(app) << "ThemeProxy constructor called";
}

ThemeProxy::~ThemeProxy()
{
    qCDebug(app) << "ThemeProxy destructor called";
}

/**
 * @brief ThemeProxy::slot_ThemeChange
 * 系统主题颜色改变时调用
 */
void ThemeProxy ::slot_ThemeChange()
{
    qCDebug(app) << "System theme changed, current theme:" << getTheme();
    //更新到前端JS
    emit themeChange(getTheme());
}

/**
 * @brief ThemeProxy::getTheme
 * @return
 * 获取系统主题颜色，白色/黑色
 */
QString ThemeProxy ::getTheme() const
{
    qCDebug(app) << "Getting current system theme";
    QString qsthemetype = "Null";
    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    if (themeType == DGuiApplicationHelper::LightType)
        qsthemetype = "LightType";
    else if (themeType == DGuiApplicationHelper::DarkType)
        qsthemetype = "DarkType";
    return qsthemetype;
}
