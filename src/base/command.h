// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INSTALLER_BASE_COMMAND_H
#define INSTALLER_BASE_COMMAND_H

#include <QStringList>

namespace dman {

// Run |cmd| with |args| in background and returns its result.
bool SpawnCmd(const QString &cmd, const QStringList &args);
bool SpawnCmd(const QString &cmd, const QStringList &args, QString &output);
bool SpawnCmd(const QString &cmd, const QStringList &args, QString &output,
              QString &err);

} // namespace dman

#endif // INSTALLER_BASE_COMMAND_H
