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

#include "view/widget/search_completion_window.h"

#include <QDebug>
#include <QVBoxLayout>

#include "view/theme_manager.h"

namespace dman {

SearchCompletionWindow::SearchCompletionWindow(QWidget* parent)
    : QFrame(parent) {
  this->setObjectName("SearchCompletionWindow");

  this->initUI();
}

SearchCompletionWindow::~SearchCompletionWindow() {
}

void SearchCompletionWindow::autoResize() {

}

void SearchCompletionWindow::goUp() {
  qDebug() << Q_FUNC_INFO;
}

void SearchCompletionWindow::goDown() {
  qDebug() << Q_FUNC_INFO;
}

void SearchCompletionWindow::initUI() {
  result_list_ = new QListView();
  result_list_->setObjectName("ResultList");
  result_list_->setMouseTracking(true);
  result_list_->setSelectionMode(QListView::SingleSelection);
  result_list_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  result_list_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  result_list_->setFocusPolicy(Qt::NoFocus);

  search_button_ = new QPushButton();
  search_button_->setObjectName("SearchButton");
  search_button_->setCheckable(true);
  search_button_->setFixedHeight(25);
  search_button_->setText(tr("Search \"%1\" in Deepin Manual"));

  QVBoxLayout* main_layout = new QVBoxLayout();
  main_layout->setContentsMargins(0, 0, 0, 0);
  main_layout->setSpacing(0);
  main_layout->addWidget(result_list_, 0, Qt::AlignHCenter | Qt::AlignTop);
  main_layout->addSpacing(1);
  main_layout->addWidget(search_button_, 0, Qt::AlignCenter);
  main_layout->addSpacing(1);

  this->setLayout(main_layout);
  this->setContentsMargins(0, 0, 0, 0);

  ThemeManager::instance()->registerWidget(this);
}

}  // namespace dman