// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "manual_open_interface.h"

/*
 * Implementation of interface class ManualOpenInterface
 */

ManualOpenInterface::ManualOpenInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

ManualOpenInterface::~ManualOpenInterface()
{
}
