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
#include <QFileInfo>
#include <QResizeEvent>
#include <QWebChannel>
#include <qcef_web_page.h>
#include <qcef_web_settings.h>
#include <qcef_web_view.h>

#include "base/consts.h"
#include "view/title_bar_proxy.h"
#include "view/widget/title_bar.h"

namespace dman {

WebWindow::WebWindow(QWidget* parent)
    : Dtk::Widget::DMainWindow(parent),
      app_name_() {

  this->initUI();

  connect(title_bar_, &TitleBar::searchTextChanged,
          this, &WebWindow::onSearchTextChanged);
  connect(web_view_->page(), &QCefWebPage::loadFinished,
          this, &WebWindow::onWebPageLoadFinished);
}

WebWindow::~WebWindow() {
}

void WebWindow::setAppName(const QString& app_name) {
  app_name_ = app_name;

  const QFileInfo info(kIndexPage);
  web_view_->load(QUrl::fromLocalFile(info.absoluteFilePath()));
}

void WebWindow::initUI() {
  title_bar_ = new TitleBar();
  title_bar_proxy_ = new TitleBarProxy(title_bar_, this);
  this->titlebar()->setCustomWidget(title_bar_, Qt::AlignCenter, false);

  web_view_ = new QCefWebView();
  this->setCentralWidget(web_view_);

  // Disable web security to allow cross-origin accessing.
  web_view_->page()->settings()->setWebSecurity(QCefWebSettings::StateDisabled);

  QWebChannel* channel = web_view_->page()->webChannel();
  // Use TitleBarProxy instead.
  channel->registerObject("titleBar", title_bar_proxy_);
}

void WebWindow::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
  title_bar_->setFixedWidth(event->size().width());
}

void WebWindow::onSearchTextChanged(const QString& text) {
  web_view_->page()->runJavaScript(QString("search(%1))").arg(text),
                                   "custom_search.js");
}

void WebWindow::onWebPageLoadFinished(bool ok) {
  if (ok) {
    if (app_name_.isEmpty()) {
      web_view_->page()->runJavaScript("index()");
    } else {
      web_view_->page()->runJavaScript(QString("open(%1)").arg(app_name_));
    }
  }
}

}  // namespace dman