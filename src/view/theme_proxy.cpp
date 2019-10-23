#include "view/theme_proxy.h"
#include <DApplicationHelper>


dman::ThemeProxy::ThemeProxy(QObject *parent) : QObject(parent)
{
}

dman::ThemeProxy::~ThemeProxy()
{
}

void dman::ThemeProxy :: slot_ThemeChange()
{
    themeChange(getTheme());
}

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
