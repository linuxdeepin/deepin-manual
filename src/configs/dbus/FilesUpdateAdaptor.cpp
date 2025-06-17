// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "FilesUpdateAdaptor.h"
#include "base/ddlog.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QDebug>

/*
 * Implementation of adaptor class FilesUpdateAdaptor
 */

FilesUpdateAdaptor::FilesUpdateAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    qCDebug(app) << "Creating FilesUpdateAdaptor instance";
    setAutoRelaySignals(true);
    qCDebug(app) << "Auto relay signals enabled";
}

FilesUpdateAdaptor::~FilesUpdateAdaptor()
{
    // destructor
    qCDebug(app) << "Destroying FilesUpdateAdaptor instance";
}

void FilesUpdateAdaptor::OnFilesUpdate(const QStringList &list)
{
    // handle method call local.ManualFilesUpdateProxy.OnFilesUpdate
    QMetaObject::invokeMethod(parent(), "OnFilesUpdate", Q_ARG(QStringList, list));
    qCDebug(app) << "Received signal OnFilesUpdate with arguments size:" << list.size();
}

