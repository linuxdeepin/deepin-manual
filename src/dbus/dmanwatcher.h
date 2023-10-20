// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DMANWATCHER_H
#define DMANWATCHER_H

#include <QObject>
#include <QTimer>

/**
 * @class DManWatcher
 * @brief 监控客户端类
 */
class DManWatcher :public QObject
{
    Q_OBJECT
public:
    explicit DManWatcher();

public Q_SLOTS:
    void onTimeOut();
private:
    QString executCmd(const QString &strCmd);
private:
    QTimer *m_Timer=nullptr;
};

#endif // DMANWATCHER_H
