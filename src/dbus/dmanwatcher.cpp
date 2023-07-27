// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "dmanwatcher.h"

#include <QProcess>
#include <QCoreApplication>
#include <QDebug>

DManWatcher::DManWatcher():m_Timer(new QTimer (this))
{

    connect(m_Timer,&QTimer::timeout,this,&DManWatcher::onTimeOut);
    m_Timer->start(5000);
}

/**
 * @brief 定时监控客户端
 */
void DManWatcher::onTimeOut()
{
    QString cmd, outPut;
    //判断dman客户端是否存在，如果不存在退出服务。
    cmd = QString("ps aux | grep -w dman$");
    outPut= executCmd(cmd);
    int ret = outPut.length();
    if (!ret)
        QCoreApplication::exit(0);
}

/**
 * @brief 执行外部命令
 * @param strCmd:外部命令字符串
 */
QString DManWatcher::executCmd(const QString &strCmd)
{
     QProcess proc;
     proc.start("bash", QStringList() << "-c" << strCmd);
     proc.waitForFinished(-1);
     return  proc.readAllStandardOutput();
}
