// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "settings_proxy.h"

#include <QtGui/QDesktopServices>
#include <QUrl>

SettingsProxy::SettingsProxy(QObject *parent)
    : QObject(parent)
{
    this->setObjectName("SettingsProxy");
}
