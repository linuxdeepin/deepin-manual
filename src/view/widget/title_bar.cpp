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

#include "view/widget/title_bar.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "view/widget/search_edit.h"

namespace dman {

TitleBar::TitleBar(QWidget* parent) : QFrame(parent) {
  this->initUI();
}

TitleBar::~TitleBar() {

}

void TitleBar::initUI() {
  app_icon_ = new QLabel();
  app_icon_->setFixedSize(24, 24);

  back_btn_ = new QPushButton();
  back_btn_->setFixedSize(24, 24);
  back_btn_->hide();

  QHBoxLayout* left_layout = new QHBoxLayout();
  left_layout->setMargin(0);
  left_layout->setSpacing(10);
  left_layout->addWidget(app_icon_, 0, Qt::AlignCenter);
  left_layout->addWidget(back_btn_, 0, Qt::AlignCenter);
  left_layout->addStretch();
  QFrame* left_widget = new QFrame();
  left_widget->setLayout(left_layout);
  left_widget->setFixedWidth(148);

  search_edit_ = new SearchEdit();
  search_edit_->setFixedWidth(278);
  search_edit_->setPlaceHolder(tr("Search"));

  QHBoxLayout* main_layout = new QHBoxLayout();
  main_layout->setContentsMargins(5, 5, 5, 5);
  main_layout->addWidget(left_widget);
  main_layout->addStretch();
  main_layout->addWidget(search_edit_);
  main_layout->addStretch();
  this->setLayout(main_layout);
}

}  // namespace dman