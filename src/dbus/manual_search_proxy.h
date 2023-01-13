// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DEEPIN_MANUAL_DBUS_MANUAL_SEARCH_PROXY_H
#define DEEPIN_MANUAL_DBUS_MANUAL_SEARCH_PROXY_H

#include <DSysInfo>

/**
 * @brief The ManualSearchProxy class
 * 用于判断是否存在给定应用名称对应的帮助手册内容
 */
class ManualSearchProxy : public QObject
{
    Q_OBJECT
public:
    explicit ManualSearchProxy(QObject *parent = nullptr);
    ~ManualSearchProxy() override;

signals:
    //窗口状态信号，打开/关闭
    void SendWinInfo(const QString &data);

public slots:
    //根据给定的应用名称，判断对应的帮助手册内容是否存在
    bool ManualExists(const QString &app_name);

private slots:
    //读取窗口状态信息
    void RecvMsg(const QString &data);
    //配合WindowManager类用于多窗口管理，帮助手册窗口的打开/激活
    void OnNewWindowOpen(const QString &data);

private:
//    void initDBus();
    //连接DBus服务，获取SendWinInfo信息
    void connectToSender();

    bool m_bWindowState;
    QString m_sApplicationPid = nullptr;
};

#endif // DEEPIN_MANUAL_DBUS_MANUAL_SEARCH_PROXY_H
