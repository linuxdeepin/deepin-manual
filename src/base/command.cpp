// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "base/command.h"

#include <DLog>

namespace dman {

bool RunScriptFile(const QStringList &args)
{
    Q_ASSERT(!args.isEmpty());
    if (args.isEmpty()) {
        qCritical() << "RunScriptFile() args is empty!";
        return false;
    }

    // Change working directory.
    const QString current_dir(QFileInfo(args.at(0)).absolutePath());
    if (!QDir::setCurrent(current_dir)) {
        qCritical() << "Failed to change working directory:" << current_dir;
        return false;
    }

    // NOTE(xushaohua): If args[0] is not a script file, bash may raise
    // error message.
    return SpawnCmd("/bin/bash", args);
}

bool RunScriptFile(const QStringList &args, QString &output, QString &err)
{
    Q_ASSERT(!args.isEmpty());
    if (args.isEmpty()) {
        qCritical() << "RunScriptFile() arg is empty!";
        return false;
    }

    // Change working directory.
    const QString current_dir(QFileInfo(args.at(0)).absolutePath());
    if (!QDir::setCurrent(current_dir)) {
        qCritical() << "Failed to change working directory:" << current_dir;
        return false;
    }

    // TODO(xushaohua): Remove bash
    return SpawnCmd("/bin/bash", args, output, err);
}

bool SpawnCmd(const QString &cmd, const QStringList &args)
{
    QProcess process;
    process.setProgram(cmd);
    process.setArguments(args);
    // Merge stdout and stderr of subprocess with main process.
    process.setProcessChannelMode(QProcess::ForwardedChannels);
    process.start();
    // Wait for process to finish without timeout.
    process.waitForFinished(-1);

    qDebug() << process.readAllStandardOutput();

    return (process.exitStatus() == QProcess::NormalExit && process.exitCode() == 0);
}

bool SpawnCmd(const QString &cmd, const QStringList &args, QString &output)
{
    QString err;
    return SpawnCmd(cmd, args, output, err);
}

bool SpawnCmd(const QString &cmd, const QStringList &args,
              QString &output, QString &err)
{
    QProcess process;
    process.setProgram(cmd);
    process.setArguments(args);
    process.setEnvironment({"LANG=en_US.UTF-8", "LANGUAGE=en_US"});
    process.start();
    // Wait for process to finish without timeout.
    process.waitForFinished(-1);
    output = process.readAllStandardOutput();
    err = process.readAllStandardError();
    if (!process.errorString().contains("Unknown")) {
        qDebug() << "run cmd error, caused by:" << process.errorString();
    }
    return (process.exitStatus() == QProcess::NormalExit && process.exitCode() == 0);
}

} // namespace dman
