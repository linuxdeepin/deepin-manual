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

// Manages visibility and lifecycle of web windows.
// Current process will exit only if all of these windows are closed.
class WindowManager : public QObject
{
    Q_OBJECT
public:
    explicit WindowManager(QObject *parent = nullptr);
    ~WindowManager() override;

    void moveWindow(WebWindow *window);
    SearchManager *currSearchManager();
    void SendMsg(const QString &msg);

private:
    QPoint newWindowPosition();
    void initDBus();
    void initWebWindow();
    void activeExistingWindow();
    void activeOrInitWindow(const QString &app_name);

    SearchManager *search_manager_ {nullptr};
    QString curr_app_name_;
    QString curr_keyword_;
    QString curr_title_name_;
    QMutex _mutex;

    /*** 2020-06-22 17:03:25 wangml ***/
    WebWindow *window = nullptr;

private slots:
    /**
     * Remove window from window list.
     * @param app_name
     */
    // void onWindowClosed(const QString &app_name);
    void onWindowShown(WebWindow *window);

public slots:
    /**
     * Open manual page of application with name |app_name|.
     * If manual of that app has already been presented, just raise to front.
     */
    void openManual(const QString &app_name, const QString &title_name);
    void openManualWithSearch(const QString &app_name, const QString &keyword);

    //void onNewAppOpen();
};

}  // namespace dman

#endif  // DEEPIN_MANUAL_CONTROLLER_WINDOW_MANAGER_H
