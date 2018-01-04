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

#ifndef DEEPIN_MANUAL_VIEW_WIDGET_TITLE_BAR_H
#define DEEPIN_MANUAL_VIEW_WIDGET_TITLE_BAR_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QStackedLayout>

namespace dman {

class SearchEdit;

// Customize widget in TitleBar.
class TitleBar : public QFrame {
  Q_OBJECT
  Q_PROPERTY(bool backButtonVisible
                 READ backButtonVisible
                 WRITE setBackButtonVisible
                 NOTIFY backButtonVisibleChanged)
 public:
  explicit TitleBar(QWidget* parent = nullptr);
  ~TitleBar() override;

  bool backButtonVisible() const;

 signals:
  void backButtonClicked();
  void backButtonVisibleChanged(bool visible);
  void searchTextChanged(const QString& text);

 public slots:
  // Show goBack button if |visible| is true.
  // Else show app icon.
  void setBackButtonVisible(bool visible);

 private:
  void initUI();

  QStackedLayout* left_layout_ = nullptr;
  QPushButton* back_btn_ = nullptr;
  SearchEdit* search_edit_ = nullptr;

 private slots:
  void onSearchTextChanged();
  void onStackedLayoutCurrentChanged(int index);
};

}  // namespace dman

#endif  // DEEPIN_MANUAL_VIEW_WIDGET_TITLE_BAR_H
