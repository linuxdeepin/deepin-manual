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

#ifndef DEEPIN_MANUAL_CONTROLLER_WINDOW_MANAGER_H
#define DEEPIN_MANUAL_CONTROLLER_WINDOW_MANAGER_H

#include <QDBusConnection>
#include <QHash>
#include <QObject>
#include <QPoint>
#include <QMutex>

namespace dman {

class SearchManager;
class WebWindow;

// 窗口管理类
class WindowManager : public QObject
{
    Q_OBJECT
public:
    explicit WindowManager(QObject *parent = nullptr);
    ~WindowManager() override;

private:
    void initDBus();
    void initWebWindow();
    void activeOrInitWindow();
    void SendMsg(const QString &msg);
    void setWindow(WebWindow *window);

    SearchManager *search_manager_ {nullptr};
    QString curr_app_name_;
    QString curr_keyword_;
    QString curr_title_name_;
    QMutex _mutex;
    WebWindow *window = nullptr;

public slots:
    /**
     * Open manual page of application with name |app_name|.
     * If manual of that app has already been presented, just raise to front.
     */
    void onNewAppOpen();
    void openManual(const QString &app_name, const QString &title_name);
    void openManualWithSearch(const QString &app_name, const QString &keyword);
};

}  // namespace dman

#endif  // DEEPIN_MANUAL_CONTROLLER_WINDOW_MANAGER_H
