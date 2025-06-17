// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Launcher1Adaptor.h"
#include "base/ddlog.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class Launcher1Adaptor
 */

Launcher1Adaptor::Launcher1Adaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    qCDebug(app) << "Creating Launcher1Adaptor instance";
    setAutoRelaySignals(true);
    qCDebug(app) << "Auto relay signals enabled for Launcher1Adaptor";
}

Launcher1Adaptor::~Launcher1Adaptor()
{
    // destructor
    qCDebug(app) << "Destroying Launcher1Adaptor instance";
}

AppInfoList Launcher1Adaptor::GetAllItemInfos()
{
    qCDebug(app) << "Handling method call org.deepin.dde.daemon.Launcher1.GetAllItemInfos";
    // handle method call org.deepin.dde.daemon.Launcher1.GetAllItemInfos
    AppInfoList out0;
    QMetaObject::invokeMethod(parent(), "GetAllItemInfos", Q_RETURN_ARG(AppInfoList, out0));
    qCDebug(app) << "GetAllItemInfos returned" << out0.size();
    return out0;
}

AppInfo Launcher1Adaptor::GetItemInfo(const QString &in0)
{
    qCDebug(app) << "Handling method call org.deepin.dde.daemon.Launcher1.GetItemInfo";
    // handle method call org.deepin.dde.daemon.Launcher1.GetItemInfo
    AppInfo out0;
    QMetaObject::invokeMethod(parent(), "GetItemInfo", Q_RETURN_ARG(AppInfo, out0), Q_ARG(QString, in0));
    qCDebug(app) << "GetItemInfo returned";
    return out0;
}

