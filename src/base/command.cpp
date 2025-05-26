// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "base/command.h"
#include "base/ddlog.h"

#include <DLog>
#include <QDir>
#include <QFileInfo>
#include <QProcess>

namespace dman {

bool RunScriptFile(const QStringList &args)
{
    qCDebug(app) << "RunScriptFile called with args:" << args;
    Q_ASSERT(!args.isEmpty());
    if (args.isEmpty()) {
        qCCritical(app) << "RunScriptFile() args is empty!";
        return false;
    }

    // Change working directory.
    const QString current_dir(QFileInfo(args.at(0)).absolutePath());
    qCDebug(app) << "Setting working directory to:" << current_dir;
    if (!QDir::setCurrent(current_dir)) {
        qCCritical(app) << "Failed to change working directory:" << current_dir;
        return false;
    }

    // NOTE(xushaohua): If args[0] is not a script file, bash may raise
    // error message.
    return SpawnCmd("/bin/bash", args);
}

bool RunScriptFile(const QStringList &args, QString &output, QString &err)
{
    qCDebug(app) << "RunScriptFile with output capture called with args:" << args;
    Q_ASSERT(!args.isEmpty());
    if (args.isEmpty()) {
         qCCritical(app) << "RunScriptFile() arg is empty!";
        return false;
    }

    // Change working directory.
    const QString current_dir(QFileInfo(args.at(0)).absolutePath());
    if (!QDir::setCurrent(current_dir)) {
         qCCritical(app) << "Failed to change working directory:" << current_dir;
        return false;
    }

    // TODO(xushaohua): Remove bash
    return SpawnCmd("/bin/bash", args, output, err);
}

bool SpawnCmd(const QString &cmd, const QStringList &args)
{
    qCDebug(app) << "Executing command:" << cmd << "args:" << args;
    QProcess process;
    process.setProgram(cmd);
    process.setArguments(args);
    // Merge stdout and stderr of subprocess with main process.
    process.setProcessChannelMode(QProcess::ForwardedChannels);
    process.start();
    qCDebug(app) << "Process started, PID:" << process.processId();

    // Wait for process to finish without timeout.
    process.waitForFinished(-1);

    qCDebug(app) << "Command output:" << process.readAllStandardOutput();

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
    qCDebug(app) << "Executing command with output capture:" << cmd << "args:" << args;
    QProcess process;
    process.setProgram(cmd);
    process.setArguments(args);
    process.setEnvironment({"LANG=en_US.UTF-8", "LANGUAGE=en_US"});
    qCDebug(app) << "Process environment set to en_US.UTF-8";

    process.start();
    qCDebug(app) << "Process started, PID:" << process.processId();

    // Wait for process to finish without timeout.
    process.waitForFinished(-1);
    output = process.readAllStandardOutput();
    err = process.readAllStandardError();
    if (!process.errorString().contains("Unknown")) {
        qCDebug(app) << "Command execution error:" << process.errorString();
    }
    return (process.exitStatus() == QProcess::NormalExit && process.exitCode() == 0);
}

} // namespace dman
