// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "base/command.h"

#include <DLog>
#include <QDir>
#include <QFileInfo>
#include <QProcess>

namespace dman {

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
