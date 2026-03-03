// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "manual_filesupdate_adapter.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class ManualFilesUpdateAdapter
 */

ManualFilesUpdateAdapter::ManualFilesUpdateAdapter(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

ManualFilesUpdateAdapter::~ManualFilesUpdateAdapter()
{
    // destructor
}

/**
 * @brief ManualFilesUpdateAdapter::OnFilesUpdate
 * @param list
 */
void ManualFilesUpdateAdapter::OnFilesUpdate(const QStringList &list)
{
    // handle method call local.ManualFilesUpdateProxy.OnFilesUpdate
    QMetaObject::invokeMethod(parent(), "OnFilesUpdate", Q_ARG(QStringList, list));
}

