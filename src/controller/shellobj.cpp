/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     wujian <wujian@uniontech.com>
* Maintainer: wujian <wujian@uniontech.com>
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
    shell->startSystemThread(cmds);
    return *shell;
}

int shellObj::startSystemThread(const QString &cmd)
{
    this->cmd = cmd;
    thread = new QThread();
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
    qDebug() << "shell " << cmd << endl;
//    emit onCompleted(iRst < 0 ? "error" : "");
    delete this;
}
