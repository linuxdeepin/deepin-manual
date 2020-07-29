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

#ifndef DEEPIN_MANUAL_DBUS_MANUAL_OPEN_PROXY_H
#define DEEPIN_MANUAL_DBUS_MANUAL_OPEN_PROXY_H

#include <QDBusConnection>
#include <QObject>

/**
 * @brief The ManualOpenProxy class
 * 用于其他应用通过按键F1调用DBus接口唤起帮助手册窗口并展示对应的帮助内容
 */
class ManualOpenProxy : public QObject
{
    Q_OBJECT
public:
    explicit ManualOpenProxy(QObject *parent = nullptr);
    ~ManualOpenProxy() override;

signals:
    void openManualRequested(const QString &app_name, const QString &title_name);
    void searchRequested(const QString &keyword);

public slots:
    void Open(const QString &app_name);
    void OpenTitle(const QString &app_name, const QString &title_name);
    void ShowManual(const QString &app_name);
    void Search(const QString &keyword);
};

#endif  // DEEPIN_MANUAL_DBUS_MANUAL_OPEN_PROXY_H
