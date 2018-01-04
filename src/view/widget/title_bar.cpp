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

#include "view/widget/search_edit.h"

namespace dman {

TitleBar::TitleBar(QWidget* parent) : QFrame(parent) {
  this->setObjectName("TitleBar");
  this->initUI();

  connect(search_edit_, &SearchEdit::textChanged, [this] {
    emit searchTextChanged(search_edit_->text());
  });
}

TitleBar::~TitleBar() {
}

void TitleBar::setBackButtonVisible(bool visible) {
  left_layout_->setCurrentIndex(visible ? 1 : 0);
}

void TitleBar::initUI() {
  QLabel* app_icon = new QLabel();
  app_icon->setObjectName("AppIcon");
  app_icon->setPixmap(QIcon(":/common/images/deepin-manual.svg").pixmap(24, 24));
  app_icon->setFixedSize(24, 24);
  QLabel* app_name = new QLabel("");
  QHBoxLayout* app_icon_layout = new QHBoxLayout();
  app_icon_layout->setContentsMargins(0, 0, 0, 0);
  app_icon_layout->setSpacing(0);
  app_icon_layout->addWidget(app_icon);
  app_icon_layout->addWidget(app_name);
  QFrame* app_icon_wrapper = new QFrame();
  app_icon_wrapper->setLayout(app_icon_layout);

  back_btn_ = new QPushButton();
  back_btn_->setObjectName("BackButton");
  back_btn_->setFixedSize(24, 24);
  back_btn_->hide();

  left_layout_ = new QStackedLayout();
  left_layout_->addWidget(app_icon_wrapper);
  left_layout_->addWidget(back_btn_);

  QFrame* left_widget = new QFrame();
  left_widget->setLayout(left_layout_);

  search_edit_ = new SearchEdit();
  search_edit_->setObjectName("SearchEdit");
  search_edit_->setFixedWidth(228);
  search_edit_->setPlaceHolder(tr("Search"));

  QHBoxLayout* main_layout = new QHBoxLayout();
  main_layout->setSpacing(0);
  main_layout->setContentsMargins(0, 0, 0, 0);
  main_layout->addWidget(left_widget);
  main_layout->addWidget(search_edit_, 1, Qt::AlignCenter);
  main_layout->addSpacing(48);
  this->setLayout(main_layout);
}

}  // namespace dman