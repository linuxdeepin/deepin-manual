// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "LauncherAdaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class LauncherAdaptor
 */

LauncherAdaptor::LauncherAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

LauncherAdaptor::~LauncherAdaptor()
{
    // destructor
}

AppInfoList LauncherAdaptor::GetAllItemInfos()
{
    // handle method call com.deepin.dde.daemon.Launcher.GetAllItemInfos
    AppInfoList out0;
    QMetaObject::invokeMethod(parent(), "GetAllItemInfos", Q_RETURN_ARG(AppInfoList, out0));
    return out0;
}

AppInfo LauncherAdaptor::GetItemInfo(const QString &in0)
{
    // handle method call com.deepin.dde.daemon.Launcher.GetItemInfo
    AppInfo out0;
    QMetaObject::invokeMethod(parent(), "GetItemInfo", Q_RETURN_ARG(AppInfo, out0), Q_ARG(QString, in0));
    return out0;
}

