// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEEPIN_MANUAL_DBUS_MANUAL_OPEN_PROXY_H
#define DEEPIN_MANUAL_DBUS_MANUAL_OPEN_PROXY_H

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
    //打开帮助手册请求，当title_name为“”时，通过应用名打开帮助手册，定位在首行概述
    void openManualRequested(const QString &app_name, const QString &title_name);
    //关键字搜索请求
    void searchRequested(const QString &keyword);

public slots:
    //通过应用名打开帮助手册，定位在首先概述
    void Open(const QString &app_name);
    //通过应用名和标签打开帮助，定位到title标签行
    void OpenTitle(const QString &app_name, const QString &title_name);
    //显示帮助手册页面， 调用open()接口实现
    void ShowManual(const QString &app_name);
    //搜索请求，发送searchRequested() 信号
    void Search(const QString &keyword);
};

#endif // DEEPIN_MANUAL_DBUS_MANUAL_OPEN_PROXY_H
