#include "view/theme_proxy.h"
#include <DApplicationHelper>

ThemeProxy::ThemeProxy(QObject *parent)
    : QObject(parent)
{
}

ThemeProxy::~ThemeProxy()
{
}

/**
 * @brief ThemeProxy::slot_ThemeChange
 * 系统主题颜色改变时调用
 */
void ThemeProxy ::slot_ThemeChange()
{
    emit themeChange(getTheme());
}

/**
 * @brief ThemeProxy::getTheme
 * @return
 * 获取系统主题颜色，白色/黑色
 */
QString ThemeProxy ::getTheme() const
{
    QString qsthemetype = "Null";
    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    if (themeType == DGuiApplicationHelper::LightType)
        qsthemetype = "LightType";
    else if (themeType == DGuiApplicationHelper::DarkType)
        qsthemetype = "DarkType";
    return qsthemetype;
}
