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

#ifndef DEEPIN_MANUAL_VIEWS_TITLE_BAR_PROXY_H
#define DEEPIN_MANUAL_VIEWS_TITLE_BAR_PROXY_H

#include <QObject>

namespace dman {

class TitleBar;

class TitleBarProxy : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool backButtonVisible
                 READ backButtonVisible
                 WRITE setBackButtonVisible)

 public:
  TitleBarProxy(TitleBar* title_bar, QObject* parent = nullptr);
  ~TitleBarProxy() override;

  bool backButtonVisible() const;

 signals:
  void backButtonClicked();

 public slots:
  // Show goBack button if |visible| is true.
  // Else show app icon.
  void setBackButtonVisible(bool visible);

 private:
  TitleBar* title_bar_ = nullptr;
};

}  // namespace dman

#endif  // DEEPIN_MANUAL_VIEWS_TITLE_BAR_PROXY_H
