/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DEEPIN_MANUAL_DBUS_MANUAL_SEARCH_PROXY_H
#define DEEPIN_MANUAL_DBUS_MANUAL_SEARCH_PROXY_H

//#include "view/manual_proxy.h"

#include <DSysInfo>
//#include <QDBusConnection>
#include <QObject>
// class LauncherInterface;
//#include <DSysInfo>

class ManualSearchProxy : public QObject
{
    Q_OBJECT
public:
    explicit ManualSearchProxy(QObject *parent = nullptr);
    ~ManualSearchProxy() override;

signals:
    void bindManual(const QString &appName, const QString &winId);
    void closeManual(const QString &app_name);

public slots:
    //    void RecvMsg(const QString &data);
    bool ManualExists(const QString &app_name);
    void BindManual(const QString &app_name, const QString &winId);
    void CloseManual(const QString &app_name);
    bool OnNewWindowOpen(const QString &data);

private:
    //    void initDBus();
    //    void connectToSender();

    //    QDBusConnection m_dbusConn;
    //    QList<QHash<QString, QString>> winInfoList;

    //    QList<QHash<QString, QString>> winInfoList;  // app---winId
};

#endif  // DEEPIN_MANUAL_DBUS_MANUAL_SEARCH_PROXY_H
