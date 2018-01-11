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
#include <QDebug>
#include <QFileInfo>
#include <QResizeEvent>
#include <QTimer>
#include <QWebChannel>
#include <qcef_web_page.h>
#include <qcef_web_settings.h>
#include <qcef_web_view.h>

#include "base/consts.h"
#include "controller/search_manager.h"
#include "view/i18n_proxy.h"
#include "view/image_viewer_proxy.h"
#include "view/search_proxy.h"
#include "view/title_bar_proxy.h"
#include "view/widget/image_viewer.h"
#include "view/widget/search_completion_window.h"
#include "view/widget/title_bar.h"
#include "view/web_event_delegate.h"

namespace dman {

WebWindow::WebWindow(SearchManager* search_manager, QWidget* parent)
    : Dtk::Widget::DMainWindow(parent),
      app_name_(),
      search_manager_(search_manager),
      search_proxy_(new SearchProxy(search_manager, this)) {

  this->initUI();
  this->initConnections();
}

WebWindow::~WebWindow() {
  if (completion_window_ != nullptr) {
    completion_window_->deleteLater();
    completion_window_ = nullptr;
  }
}

void WebWindow::setAppName(const QString& app_name) {
  app_name_ = app_name;

  const QFileInfo info(kIndexPage);
  web_view_->load(QUrl::fromLocalFile(info.absoluteFilePath()));
}

void WebWindow::initConnections() {
  connect(title_bar_, &TitleBar::searchTextChanged,
          this, &WebWindow::onSearchTextChanged);
  connect(web_view_->page(), &QCefWebPage::loadFinished,
          this, &WebWindow::onWebPageLoadFinished);
  connect(title_bar_, &TitleBar::downKeyPressed,
          completion_window_, &SearchCompletionWindow::goDown);
  connect(title_bar_, &TitleBar::enterPressed,
          completion_window_, &SearchCompletionWindow::onEnterPressed);
  connect(title_bar_, &TitleBar::upKeyPressed,
          completion_window_, &SearchCompletionWindow::goUp);
  connect(title_bar_, &TitleBar::focusOut,
          this, &WebWindow::onSearchEditFocusOut);

  connect(search_manager_, &SearchManager::searchAnchorResult,
          this, &WebWindow::onSearchAnchorResult);
  connect(search_manager_, &SearchManager::searchContentResult,
          search_proxy_, &SearchProxy::onContentResult);
  connect(completion_window_, &SearchCompletionWindow::resultClicked,
          this, &WebWindow::onSearchResultClicked);
  connect(completion_window_, &SearchCompletionWindow::searchButtonClicked,
          this, &WebWindow::onSearchButtonClicked);
}

void WebWindow::initUI() {
  i18n_ = new I18nProxy(this);

  completion_window_ = new SearchCompletionWindow();
  completion_window_->hide();

  title_bar_ = new TitleBar();
  title_bar_proxy_ = new TitleBarProxy(title_bar_, this);
  this->titlebar()->setCustomWidget(title_bar_, Qt::AlignCenter, false);
  this->titlebar()->setSeparatorVisible(true);

  image_viewer_ = new ImageViewer(this);
  image_viewer_proxy_ = new ImageViewerProxy(image_viewer_, this);

  web_view_ = new QCefWebView();
  web_event_delegate_ = new WebEventDelegate(this);
  web_view_->page()->setEventDelegate(web_event_delegate_);
  this->setCentralWidget(web_view_);

  // Disable web security.
  web_view_->page()->settings()->setWebSecurity(QCefWebSettings::StateDisabled);

  // Use TitleBarProxy instead.
  QWebChannel* channel = web_view_->page()->webChannel();
  channel->registerObject("i18n", i18n_);
  channel->registerObject("imageViewer", image_viewer_proxy_);
  channel->registerObject("search", search_proxy_);
  channel->registerObject("titleBar", title_bar_proxy_);

  this->setFocusPolicy(Qt::ClickFocus);
}

void WebWindow::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
  title_bar_->setFixedWidth(event->size().width());
}

void WebWindow::onSearchEditFocusOut() {
  QTimer::singleShot(50, [=]() {
    this->completion_window_->hide();
  });
}

void WebWindow::onSearchButtonClicked() {
  const QString keyword = title_bar_->getSearchText();
  search_manager_->searchContent(keyword);

  // Show search page.
  web_view_->page()->runJavaScript(
      QString("openSearchPage('%1')").arg(keyword));
}

void WebWindow::onSearchResultClicked(const QString& app_name,
                                      const QString& anchor) {
  web_view_->page()->runJavaScript(
      QString("openApp('%1', '%2')").arg(app_name).arg(anchor));
}

void WebWindow::onSearchTextChanged(const QString& text) {
  if (text.size() > 1) {
    // Do real search.
    completion_window_->setKeyword(text);
    completion_window_->show();
    completion_window_->autoResize();
    // Move to below of search edit.
    const QPoint local_point(this->rect().width() / 2 - 120, 36);
    const QPoint global_point(this->mapToGlobal(local_point));
    completion_window_->move(global_point);
    completion_window_->setFocusPolicy(Qt::StrongFocus);
    completion_window_->raise();
    search_manager_->searchAnchor(text);
  } else {
    this->onSearchEditFocusOut();
  }
}

void WebWindow::onWebPageLoadFinished(bool ok) {
  if (ok) {
    if (app_name_.isEmpty()) {
      web_view_->page()->runJavaScript("index()");
    } else {
      QString real_path(app_name_);
      if (real_path.contains('/')) {
        // Open markdown file with absolute path.
        QFileInfo info(real_path);
        real_path = info.canonicalFilePath();
        web_view_->page()->runJavaScript(
            QString("openFile(\"%1\")").arg(real_path));
      } else {
        // Open system manual.
        web_view_->page()->runJavaScript(
            QString("openApp(\"%1\")").arg(app_name_));
      }
    }
  }
}

void WebWindow::onSearchAnchorResult(const QString& keyword,
                                     const SearchAnchorResultList& result) {
  Q_UNUSED(keyword);
  completion_window_->setSearchAnchorResult(result);
}

}  // namespace dman