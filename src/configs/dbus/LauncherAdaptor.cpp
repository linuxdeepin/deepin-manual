// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "LauncherAdaptor.h"
#include "base/ddlog.h"

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
    qCDebug(app) << "Creating LauncherAdaptor instance";
    setAutoRelaySignals(true);
    qCDebug(app) << "Auto relay signals enabled for LauncherAdaptor";
}

LauncherAdaptor::~LauncherAdaptor()
{
    // destructor
    qCDebug(app) << "Destroying LauncherAdaptor instance";
}

AppInfoList LauncherAdaptor::GetAllItemInfos()
{
    qCDebug(app) << "Calling GetAllItemInfos method";
    // handle method call com.deepin.dde.daemon.Launcher.GetAllItemInfos
    AppInfoList out0;
    QMetaObject::invokeMethod(parent(), "GetAllItemInfos", Q_RETURN_ARG(AppInfoList, out0));
    qCDebug(app) << "GetAllItemInfos method returned " << out0.size();
    return out0;
}

AppInfo LauncherAdaptor::GetItemInfo(const QString &in0)
{
    qCDebug(app) << "Calling GetItemInfo method with argument " << in0;
    // handle method call com.deepin.dde.daemon.Launcher.GetItemInfo
    AppInfo out0;
    QMetaObject::invokeMethod(parent(), "GetItemInfo", Q_RETURN_ARG(AppInfo, out0), Q_ARG(QString, in0));
    qCDebug(app) << "GetItemInfo method returned";
    return out0;
}

