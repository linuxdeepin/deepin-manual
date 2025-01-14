// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "shellobj.h"
#include <QDebug>
#include <QProcess>


shellObj::shellObj()
{
    thread = nullptr;
}

shellObj::~shellObj()
{
    if (thread != nullptr) {
        thread->quit();
    }
    qDebug() << "thread exit";
    thread = nullptr;
}

shellObj &shellObj::execSystem(const QString &cmds)
{
    shellObj *shell = new shellObj();
    qDebug() << "shell exec:" << cmds;
    //执行shell命令
    shell->startSystemThread(cmds);
    return *shell;
}

int shellObj::startSystemThread(const QString &cmd)
{
    this->cmd = cmd;
    thread = new QThread();
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    moveToThread(thread);
    connect(thread, &QThread::started, [ = ]() {
        runSystem();
    });
    thread->start();
    return 0;
}

void shellObj::runSystem()
{
    system(cmd.toStdString().data());
    qDebug() << "shell " << cmd;
//    emit onCompleted(iRst < 0 ? "error" : "");
    delete this;
}
