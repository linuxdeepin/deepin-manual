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

#ifndef DEEPIN_MANUAL_VIEW_MANUAL_PROXY_H
#define DEEPIN_MANUAL_VIEW_MANUAL_PROXY_H

#include <QObject>
#include <QList>

class LauncherInterface;

namespace dman {

class ManualProxy : public QObject {
  Q_OBJECT
 public:
  explicit ManualProxy(QObject* parent = nullptr);
  ~ManualProxy() override;

signals:
   void WidgetLower();
 public slots:
  QString getSystemManualDir() const {
    return DMAN_MANUAL_DIR;
  };

  QStringList getSystemManualList();

  /**
   * Open link with xdg-open.
   * @param url
   */
  void openExternalLink(const QString& url);


 private:
  LauncherInterface* launcher_interface_ = nullptr;
  QStringList app_list_;
};

}  // namespace dman

#endif  // DEEPIN_MANUAL_VIEW_MANUAL_PROXY_H
