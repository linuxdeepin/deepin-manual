// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SearchAdaptor.h"
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
    // constructor
    setAutoRelaySignals(true);
}

SearchAdaptor::~SearchAdaptor()
{
    // destructor
}

bool SearchAdaptor::ManualExists(const QString &in0)
{
    // handle method call com.deepin.Manual.Search.ManualExists
    bool out0;
    QMetaObject::invokeMethod(parent(), "ManualExists", Q_RETURN_ARG(bool, out0), Q_ARG(QString, in0));
    return out0;
}

