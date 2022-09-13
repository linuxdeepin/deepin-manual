// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEEPIN_MANUAL_CONTROLLER_WINDOW_MANAGER_H
#define DEEPIN_MANUAL_CONTROLLER_WINDOW_MANAGER_H

#include <QObject>
#include <QMutex>


class WebWindow;

// 窗口管理类
class WindowManager : public QObject
{
    Q_OBJECT
public:
    explicit WindowManager(QObject *parent = nullptr);
    ~WindowManager() override;

    //获取系统启动时间
    void setStartTime(qint64 startTime);
private:
    void initDBus();
    void initWebWindow();
    void activeOrInitWindow();
    void SendMsg(const QString &msg);
    void setWindow(WebWindow *window);

    QString curr_app_name_;
    QString curr_keyword_;
    QString curr_title_name_;
    QMutex _mutex;
    WebWindow *window = nullptr;
    qint64 appStartTime;

public slots:
    void openManual(const QString &app_name, const QString &title_name);
    void openManualWithSearch(const QString &app_name, const QString &keyword);
    //文件更新提示
    void onFilesUpdate(const QStringList &filesList);
};


#endif // DEEPIN_MANUAL_CONTROLLER_WINDOW_MANAGER_H
