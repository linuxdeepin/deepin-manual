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
    QStringList args;
    //判断dman客户端是否存在，如果不存在退出服务。
    cmd = "ps";
    args << "aux";
    outPut= executCmd(cmd, args);
    bool bHasDman = false;
    QStringList rows = outPut.split('\n');
    for (auto line : rows) {
        QStringList items = line.split(' ');
        if (items.contains("dman")) {
            bHasDman = true;
            break;
        }
    }
    if (!bHasDman)
        QCoreApplication::exit(0);
}

/**
 * @brief 执行外部命令
 * @param strCmd:外部命令字符串
 */
QString DManWatcher::executCmd(const QString &strCmd, const QStringList &args)
{
     QProcess proc;
     proc.start(strCmd, args);
     proc.waitForFinished(-1);
     return  proc.readAllStandardOutput();
}
