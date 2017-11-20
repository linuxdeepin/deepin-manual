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

#include "view/web_window.h"

#include <DTitlebar>
#include <qcef_web_view.h>

#include "resources/images/images.h"
#include "view/widget/title_bar.h"

namespace dman {

WebWindow::WebWindow(QWidget* parent) : Dtk::Widget::DMainWindow(parent),
                                        app_name_() {
  this->initUI();
}

WebWindow::~WebWindow() {

}

void WebWindow::setAppName(const QString& app_name) {
  if (app_name_ == app_name) {
    return;
  }
  app_name_ = app_name;
  web_view_->load(QUrl("https://www.bing.com"));
}

void WebWindow::initUI() {
  title_bar_ = new TitleBar();
  this->titlebar()->setCustomWidget(title_bar_, Qt::AlignLeft, false);

  web_view_ = new QCefWebView();
  this->setCentralWidget(web_view_);
}

}  // namespace dman