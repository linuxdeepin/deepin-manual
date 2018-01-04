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

#include <QFileInfo>

#include <DTitlebar>
#include <qcef_web_page.h>
#include <qcef_web_settings.h>
#include <qcef_web_view.h>

#include "base/consts.h"
#include "resources/themes/images.h"
#include "view/widget/title_bar.h"

namespace dman {

WebWindow::WebWindow(QWidget* parent) : Dtk::Widget::DMainWindow(parent),
                                        app_name_() {
  this->initUI();

  connect(title_bar_, &TitleBar::searchTextChanged, [this](const QString &text) {
    web_view_->page()->runJavaScript("search(\"" + text + "\")", "custom_search.js");
  });
}

WebWindow::~WebWindow() {

}

void WebWindow::setAppName(const QString& app_name) {
  app_name_ = app_name;

  QFileInfo fInfo(kIndexPage);
  web_view_->load(QUrl::fromLocalFile(fInfo.absoluteFilePath()));

  QCefWebPage *page = web_view_->page();
  connect(page, &QCefWebPage::loadFinished, [this, page](bool ok){
    if (ok) {
      if (app_name_.isEmpty()) {
        page->runJavaScript("index()");
      } else {
        page->runJavaScript("open(\"" + app_name_ + "\")");
      }
    }
  });
}

void WebWindow::initUI() {
  title_bar_ = new TitleBar();
  this->titlebar()->setCustomWidget(title_bar_, Qt::AlignLeft, false);

  web_view_ = new QCefWebView();
  this->setCentralWidget(web_view_);

  // Disable web security to allow cross-origin accessing.
  web_view_->page()->settings()->setWebSecurity(QCefWebSettings::StateDisabled);
}

}  // namespace dman