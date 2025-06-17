// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SearchAdaptor.h"
#include "base/ddlog.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class SearchAdaptor
 */

SearchAdaptor::SearchAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    qCDebug(app) << "SearchAdaptor constructor called";
    setAutoRelaySignals(true);
    qCDebug(app) << "Auto relay signals enabled";
}

SearchAdaptor::~SearchAdaptor()
{
    qCDebug(app) << "Destroying SearchAdaptor instance";
}

bool SearchAdaptor::ManualExists(const QString &in0)
{
    qCDebug(app) << "ManualExists method called with parameter:" << in0;
    // handle method call com.deepin.Manual.Search.ManualExists
    bool out0;
    QMetaObject::invokeMethod(parent(), "ManualExists", Q_RETURN_ARG(bool, out0), Q_ARG(QString, in0));
    qCDebug(app) << "ManualExists result:" << out0;
    return out0;
}

