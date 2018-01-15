/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include <QObject>
#include <QPoint>
#include <QVector>

namespace dman {

class SearchManager;
class WebWindow;

// Manages visibility and lifecycle of web windows.
// Current process will exit only if all of these windows are closed.
class WindowManager : public QObject {
  Q_OBJECT
 public:
  explicit WindowManager(QObject* parent = nullptr);
  ~WindowManager() override;

  // Open manual page of application with name |app_name|.
  // If manual of that app has already been presented, just raise to front.
  void openManual(const QString& app_name);

  bool parseArguments();

 private:
  QPoint newWindowPosition();

  QVector<WebWindow*> windows_;
  SearchManager* search_manager_ = nullptr;
  QPoint last_new_window_pos_;
};

}  // namespace dman

#endif  // DEEPIN_MANUAL_CONTROLLER_WINDOW_MANAGER_H
