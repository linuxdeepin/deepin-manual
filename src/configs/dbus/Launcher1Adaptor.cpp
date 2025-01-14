// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Launcher1Adaptor.h"
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
    setAutoRelaySignals(true);
}

Launcher1Adaptor::~Launcher1Adaptor()
{
    // destructor
}

AppInfoList Launcher1Adaptor::GetAllItemInfos()
{
    // handle method call org.deepin.dde.daemon.Launcher1.GetAllItemInfos
    AppInfoList out0;
    QMetaObject::invokeMethod(parent(), "GetAllItemInfos", Q_RETURN_ARG(AppInfoList, out0));
    return out0;
}

AppInfo Launcher1Adaptor::GetItemInfo(const QString &in0)
{
    // handle method call org.deepin.dde.daemon.Launcher1.GetItemInfo
    AppInfo out0;
    QMetaObject::invokeMethod(parent(), "GetItemInfo", Q_RETURN_ARG(AppInfo, out0), Q_ARG(QString, in0));
    return out0;
}

