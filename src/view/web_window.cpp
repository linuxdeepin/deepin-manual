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

#include "view/web_window.h"

#include <DTitlebar>
#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QWebChannel>
#include <qcef_web_page.h>
#include <qcef_web_settings.h>
#include <qcef_web_view.h>
#include <DButtonBox>
#include <DApplicationHelper>

#include "base/consts.h"
#include "controller/search_manager.h"
#include "view/i18n_proxy.h"
#include "view/image_viewer_proxy.h"
#include "view/manual_proxy.h"
#include "view/search_proxy.h"
#include "view/theme_proxy.h"
#include "view/title_bar_proxy.h"
#include "view/web_event_delegate.h"
#include "view/widget/image_viewer.h"
#include "view/widget/search_completion_window.h"
#include "view/widget/title_bar.h"
#include "view/widget/search_edit.h"

DWIDGET_USE_NAMESPACE

namespace dman {

namespace {

const int kSearchDelay = 200;

}  // namespace

WebWindow::WebWindow(SearchManager *search_manager, QWidget *parent)
    : Dtk::Widget::DMainWindow(parent),
      app_name_(),
      search_manager_(search_manager),
      search_proxy_(new SearchProxy(this)),
      theme_proxy_(new ThemeProxy(this)),
      search_timer_()
{

    search_timer_.setSingleShot(true);
    this->initUI();
    this->initConnections();

    qApp->installEventFilter(this);

}

WebWindow::~WebWindow()
{
    if (completion_window_ != nullptr) {
        delete completion_window_;
        completion_window_ = nullptr;
    }
}

void WebWindow::setAppName(const QString &app_name)
{
    app_name_ = app_name;

    const QFileInfo info(kIndexPage);
    web_view_->load(QUrl::fromLocalFile(info.absoluteFilePath()));
}

void WebWindow::initConnections()
{
    connect(search_edit_, &SearchEdit::textChanged,
            this, &WebWindow::onSearchTextChanged);
    connect(web_view_->page(), &QCefWebPage::loadFinished,
            this, &WebWindow::onWebPageLoadFinished);
    connect(search_edit_, &SearchEdit::downKeyPressed,
            completion_window_, &SearchCompletionWindow::goDown);
    connect(search_edit_, &SearchEdit::enterPressed,
            this, &WebWindow::onTitleBarEntered);
    connect(search_edit_, &SearchEdit::upKeyPressed,
            completion_window_, &SearchCompletionWindow::goUp);
    connect(search_edit_, &SearchEdit::focusChanged,
            this, &WebWindow::onSearchEditFocusOut);

    connect(search_manager_, &SearchManager::searchAnchorResult,
            this, &WebWindow::onSearchAnchorResult);
    connect(search_manager_, &SearchManager::searchContentResult,
            search_proxy_, &SearchProxy::onContentResult);
    connect(search_manager_, &SearchManager::searchContentMismatch,
            search_proxy_, &SearchProxy::mismatch);
    connect(completion_window_, &SearchCompletionWindow::resultClicked,
            this, &WebWindow::onSearchResultClicked);
    connect(completion_window_, &SearchCompletionWindow::searchButtonClicked,
            this, &WebWindow::onSearchButtonClicked);
    connect(&search_timer_, &QTimer::timeout,
            this, &WebWindow::onSearchTextChangedDelay);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            theme_proxy_, &ThemeProxy::slot_ThemeChange);
    connect(title_bar_proxy_, &TitleBarProxy::buttonShowSignal,
            this, &WebWindow::slot_ButtonShow);
}

void WebWindow::initUI()
{
    i18n_ = new I18nProxy(this);

    completion_window_ = new SearchCompletionWindow();
    completion_window_->hide();

    //init custom title
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(0);

    m_backButton = new DButtonBoxButton(DStyle::standardIcon(this->style(), DStyle::SP_ArrowLeave));
    m_backButton->setDisabled(true);
    m_backButton->setFixedSize(36, 36);

    m_forwardButton = new DButtonBoxButton(DStyle::standardIcon(this->style(), DStyle::SP_ArrowEnter));
    m_forwardButton->setDisabled(true);
    m_forwardButton->setFixedSize(36, 36);

    QList<DButtonBoxButton *> buttonList;
    buttonList << m_backButton << m_forwardButton;

    buttonBox = new Dtk::Widget::DButtonBox(this);
    buttonBox->setButtonList(buttonList, true);
    buttonBox->setFocusPolicy(Qt::NoFocus);

    buttonLayout->addWidget(buttonBox);
    buttonLayout->setSpacing(0);
    buttonLayout->setContentsMargins(13, 0, 0, 0);

    QFrame *buttonFrame = new QFrame(this);
    buttonFrame->setLayout(buttonLayout);

    search_edit_ = new SearchEdit(this);
    QFont searchFont;
    searchFont.setPixelSize(14);
    search_edit_->setFont(searchFont);
    search_edit_->setObjectName("SearchEdit");
    search_edit_->setFixedSize(350, 44);
    search_edit_->setPlaceHolder(QObject::tr("Search"));

    this->titlebar()->addWidget(buttonFrame, Qt::AlignLeft);
    this->titlebar()->addWidget(search_edit_, Qt::AlignCenter);

    //
    title_bar_proxy_ = new TitleBarProxy(this);
    connect(m_backButton, &DButtonBoxButton::clicked,
            title_bar_proxy_, &TitleBarProxy::backwardButtonClicked);
    connect(m_forwardButton, &DButtonBoxButton::clicked,
            title_bar_proxy_, &TitleBarProxy::forwardButtonClicked);

    this->titlebar()->setSeparatorVisible(true);
    this->titlebar()->setIcon(QIcon::fromTheme("deepin-manual"));

    image_viewer_ = new ImageViewer(this);
    image_viewer_proxy_ = new ImageViewerProxy(image_viewer_, this);

    manual_proxy_ = new ManualProxy(this);
    connect(manual_proxy_, &ManualProxy::WidgetLower, this, &WebWindow::lower);

    web_view_ = new QCefWebView();
    web_view_->page()->setEventDelegate(new WebEventDelegate(this));
    this->setCentralWidget(web_view_);

    // Disable web security.
    web_view_->page()->settings()->setWebSecurity(QCefWebSettings::StateDisabled);

    // Use TitleBarProxy instead.
    QWebChannel *channel = web_view_->page()->webChannel();
    web_view_->setAcceptDrops(false);
    channel->registerObject("i18n", i18n_);
    channel->registerObject("imageViewer", image_viewer_proxy_);
    channel->registerObject("manual", manual_proxy_);
    channel->registerObject("search", search_proxy_);
    channel->registerObject("theme", theme_proxy_);
    channel->registerObject("titleBar", title_bar_proxy_);

    this->setFocusPolicy(Qt::ClickFocus);
}

void WebWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void WebWindow::onSearchEditFocusOut()
{
    QTimer::singleShot(20, [ = ]() {
        this->completion_window_->hide();
    });
}

void WebWindow::onSearchButtonClicked()
{
    QString text = search_edit_->text();
    const QString keyword = text.remove('\n').remove('\r').remove("\r\n");
    search_manager_->searchContent(keyword);

    // Show search page.
    web_view_->page()->runJavaScript(
        QString("openSearchPage('%1')").arg(keyword));
}

void WebWindow::onSearchResultClicked(const SearchAnchorResult &result)
{
    web_view_->page()->runJavaScript(
        QString("open('%1', '%2', '%3')")
        .arg(result.app_name)
        .arg(result.anchorId)
        .arg(result.anchor));
}

void WebWindow::onSearchTextChanged(const QString &text)
{
    if (text.size() >= 1) {
        search_timer_.stop();
        search_timer_.start(kSearchDelay);
    } else {
        this->onSearchEditFocusOut();
    }
}

void WebWindow::onSearchTextChangedDelay()
{
    QString textTemp = search_edit_->text();
    const QString text = textTemp.remove('\n').remove('\r').remove("\r\n");
    // Filters special chars.
    if (text.size() < 1 || text.contains(QRegExp("[+-_$!@#%^&\\(\\)]"))) {
        return;
    }

    completion_window_->setKeyword(text);

    // Do real search.
    search_manager_->searchAnchor(text);
}

void WebWindow::onTitleBarEntered()
{
    QString textTemp = search_edit_->text();
    const QString text = textTemp.remove('\n').remove('\r').remove("\r\n");
    if (text.size() >= 1) {
        completion_window_->onEnterPressed();
    }
}

void WebWindow::onWebPageLoadFinished(bool ok)
{
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
                    QString("open('%1')").arg(real_path));
            } else {
                // Open system manual.
                web_view_->page()->runJavaScript(
                    QString("open('%1')").arg(app_name_));
            }
        }
    }
}

void WebWindow::onSearchAnchorResult(const QString &keyword,
                                     const SearchAnchorResultList &result)
{
    // Ignore this signal if current manual window is not present.
    if (keyword != completion_window_->keyword()) {
        return;
    }

    Q_UNUSED(keyword);
    if (result.isEmpty()) {
        // Hide completion window if no anchor entry matches.
        completion_window_->hide();
    } else {
        completion_window_->show();
        completion_window_->raise();
        completion_window_->autoResize();
        // Move to below of search edit.
        const QPoint local_point(this->rect().width() / 2 - search_edit_->width() / 2, 50);
        const QPoint global_point(this->mapToGlobal(local_point));
        completion_window_->move(global_point);
        completion_window_->setFocusPolicy(Qt::NoFocus);
        completion_window_->setFocusPolicy(Qt::StrongFocus);
        completion_window_->setSearchAnchorResult(result);
    }
}

bool WebWindow::eventFilter(QObject *watched, QEvent *event)
{
    // Filters mouse press event only.
    if (event->type() == QEvent::MouseButtonPress &&
            qApp->activeWindow() == this &&
            watched->objectName() == QLatin1String("QMainWindowClassWindow")) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        switch (mouseEvent->button()) {
        case Qt::BackButton: {
            title_bar_proxy_->backwardButtonClicked();
            break;
        }
        case Qt::ForwardButton: {
            title_bar_proxy_->forwardButtonClicked();
            break;
        }
        default: {
        }
        }
    }
    return QObject::eventFilter(watched, event);
}

void WebWindow::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
    emit this->closed(app_name_);
}

void WebWindow::slot_ButtonHide()
{
    qDebug() << "slot_ButtonHide";
    buttonBox->hide();
}

void WebWindow::slot_ButtonShow()
{
    qDebug() << "slot_ButtonShow";
    buttonBox->show();
}

}  // namespace dman

