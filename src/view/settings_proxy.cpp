// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "settings_proxy.h"

#include <QtGui/QDesktopServices>
#include <QUrl>

SettingsProxy::SettingsProxy(QObject *parent)
    : QObject(parent)
{
    this->setObjectName("SettingsProxy");
}
