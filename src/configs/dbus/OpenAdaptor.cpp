// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "OpenAdaptor.h"
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
    // constructor
    setAutoRelaySignals(true);
}

OpenAdaptor::~OpenAdaptor()
{
    // destructor
}

void OpenAdaptor::Open(const QString &in0)
{
    // handle method call com.deepin.Manual.Open.Open
    QMetaObject::invokeMethod(parent(), "Open", Q_ARG(QString, in0));
}

void OpenAdaptor::Search(const QString &keyword)
{
    // handle method call com.deepin.Manual.Open.Search
    QMetaObject::invokeMethod(parent(), "Search", Q_ARG(QString, keyword));
}

void OpenAdaptor::ShowManual(const QString &in0)
{
    // handle method call com.deepin.Manual.Open.ShowManual
    QMetaObject::invokeMethod(parent(), "ShowManual", Q_ARG(QString, in0));
}

