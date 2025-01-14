// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "FilesUpdateAdaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class FilesUpdateAdaptor
 */

FilesUpdateAdaptor::FilesUpdateAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

FilesUpdateAdaptor::~FilesUpdateAdaptor()
{
    // destructor
}

void FilesUpdateAdaptor::OnFilesUpdate(const QStringList &list)
{
    // handle method call local.ManualFilesUpdateProxy.OnFilesUpdate
    QMetaObject::invokeMethod(parent(), "OnFilesUpdate", Q_ARG(QStringList, list));
}

