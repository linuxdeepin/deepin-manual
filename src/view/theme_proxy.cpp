#include "view/theme_proxy.h"
#include <DApplicationHelper>


dman::ThemeProxy::ThemeProxy(QObject *parent) : QObject(parent)
{
}

dman::ThemeProxy::~ThemeProxy()
{
}

/**
 * @brief dman::ThemeProxy::slot_ThemeChange
 * 系统主题颜色改变时调用
 */
void dman::ThemeProxy :: slot_ThemeChange()
{
    emit themeChange(getTheme());
}

/**
 * @brief dman::ThemeProxy::getTheme
 * @return
 * 获取系统主题颜色，白色/黑色
 */
QString dman::ThemeProxy :: getTheme() const
{
    QString qsthemetype = "Null";
    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    if (themeType == DGuiApplicationHelper::LightType)
        qsthemetype = "LightType";
    else if (themeType == DGuiApplicationHelper::DarkType)
        qsthemetype = "DarkType";
    return qsthemetype;
}
