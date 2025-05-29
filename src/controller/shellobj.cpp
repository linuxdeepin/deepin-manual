// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "shellobj.h"
#include "base/ddlog.h"

#include <QDebug>
#include <QProcess>

shellObj::shellObj()
{
    qCDebug(app) << "Creating shellObj instance";
    thread = nullptr;
}

shellObj::~shellObj()
{
    qCDebug(app) << "Destroying shellObj";
    if (thread != nullptr) {
        qCDebug(app) << "Stopping shell thread";
        thread->quit();
    }
    thread = nullptr;
    qCDebug(app) << "shellObj destroyed";
}

shellObj &shellObj::execSystem(const QString &cmds)
{
    shellObj *shell = new shellObj();
    qCDebug(app) << "Executing shell command:" << cmds;
    //执行shell命令
    shell->startSystemThread(cmds);
    return *shell;
}

int shellObj::startSystemThread(const QString &cmd)
{
    qCDebug(app) << "Starting shell command thread";
    this->cmd = cmd;

    thread = new QThread();
    qCDebug(app) << "Created shell thread:" << thread;

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    moveToThread(thread);
    connect(thread, &QThread::started, [ = ]() {
        qCDebug(app) << "Shell thread started, running command:" << cmd;
        runSystem();
    });

    thread->start();
    qCDebug(app) << "Shell thread started successfully";
    return 0;
}

void shellObj::runSystem()
{
    system(cmd.toStdString().data());
    qCDebug(app) << "shell " << cmd;
//    emit onCompleted(iRst < 0 ? "error" : "");
    delete this;
}
