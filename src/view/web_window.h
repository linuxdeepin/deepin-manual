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

#ifndef DEEPIN_MANUAL_VIEW_WEB_WINDOW_H
#define DEEPIN_MANUAL_VIEW_WEB_WINDOW_H

#include <QFrame>

class QCefWebView;

namespace dman {

class WebWindow : public QFrame {
  Q_OBJECT
  Q_PROPERTY(QString appName READ appName WRITE setAppName)

 public:
  explicit WebWindow(QWidget* parent = nullptr);
  ~WebWindow() override;

  // Get app name of manual currently presented.
  const QString& appName() const {
    return app_name_;
  }

 public slots:
  void setAppName(const QString& app_name);

 private:
  void initUI();

  QString app_name_;
  QCefWebView* web_view_ = nullptr;
};

}  // namespace dman

#endif  // DEEPIN_MANUAL_VIEW_WEB_WINDOW_H
