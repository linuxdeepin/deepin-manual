// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "OpenAdaptor.h"
#include "base/ddlog.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>


/*
 * Implementation of adaptor class OpenAdaptor
 */

OpenAdaptor::OpenAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    qCDebug(app) << "OpenAdaptor constructor called";
    setAutoRelaySignals(true);
    qCDebug(app) << "Auto relay signals enabled";
}

OpenAdaptor::~OpenAdaptor()
{
    // destructor
}

void OpenAdaptor::Open(const QString &in0)
{
    qCDebug(app) << "DBus Open method called with parameter:" << in0;
    // handle method call com.deepin.Manual.Open.Open
    QMetaObject::invokeMethod(parent(), "Open", Q_ARG(QString, in0));
    qCDebug(app) << "Open method invocation completed";
}

void OpenAdaptor::Search(const QString &keyword)
{
    qCDebug(app) << "DBus Search method called with keyword:" << keyword;
    // handle method call com.deepin.Manual.Open.Search
    QMetaObject::invokeMethod(parent(), "Search", Q_ARG(QString, keyword));
    qCDebug(app) << "Search method invocation completed";
}

void OpenAdaptor::ShowManual(const QString &in0)
{
    qCDebug(app) << "DBus ShowManual method called with parameter:" << in0;
    // handle method call com.deepin.Manual.Open.ShowManual
    QMetaObject::invokeMethod(parent(), "ShowManual", Q_ARG(QString, in0));
    qCDebug(app) << "ShowManual method invocation completed";
}

