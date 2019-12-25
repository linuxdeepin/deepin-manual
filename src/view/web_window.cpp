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
#include "base/consts.h"
#include "dbus/dbus_consts.h"
#include "controller/search_manager.h"
#include "view/i18n_proxy.h"
#include "view/image_viewer_proxy.h"
#include "view/manual_proxy.h"
#include "view/search_proxy.h"
#include "view/theme_proxy.h"
#include "view/title_bar_proxy.h"
#include "view/settings_proxy.h"
#include "view/web_event_delegate.h"
#include "view/widget/image_viewer.h"
#include "view/widget/search_completion_window.h"
#include "view/widget/search_edit.h"
#include "base/command.h"
#include "base/utils.h"

#include <qcef_web_page.h>

#include <QShortcut>
#include <QWindow>
#include <QApplication>
#include <QFileInfo>
#include <QMouseEvent>
#include <QShowEvent>
#include <QWebChannel>
#include <QDBusConnection>
#include <QX11Info>

#include <DLog>
#include <DTitlebar>
#include <DButtonBox>
#include <DApplicationHelper>
#include <DPlatformWindowHandle>

DWIDGET_USE_NAMESPACE


namespace dman {

namespace {

const int kSearchDelay = 200;

}  // namespace


WebWindow::WebWindow(QWidget *parent)
    : Dtk::Widget::DMainWindow(parent)
    , app_name_()
    , search_timer_()
    , keyword_("")
    , first_webpage_loaded_(true)
{
    // 使用 redirectContent 模式，用于内嵌 x11 窗口时能有正确的圆角效果
    Dtk::Widget::DPlatformWindowHandle::enableDXcbForWindow(this, true);

    search_timer_.setSingleShot(true);

    this->initUI();
    this->initConnections();
    this->initShortcuts();

    qApp->installEventFilter(this);
}

WebWindow::~WebWindow()
{
    if (completion_window_ != nullptr) {
        delete completion_window_;
        completion_window_ = nullptr;
    }

    if (image_viewer_proxy_ != nullptr) {
        image_viewer_proxy_->deleteLater();
        image_viewer_proxy_ = nullptr;
    }

    if (search_proxy_ != nullptr) {
        search_proxy_->deleteLater();
        search_proxy_ = nullptr;
    }

    if (theme_proxy_ != nullptr) {
        theme_proxy_->deleteLater();
        theme_proxy_ = nullptr;
    }

    if (manual_proxy_ != nullptr) {
        manual_proxy_->deleteLater();
        manual_proxy_ = nullptr;
    }

    if (title_bar_proxy_ != nullptr) {
        title_bar_proxy_->deleteLater();
        title_bar_proxy_ = nullptr;
    }

    if (settings_proxy_ != nullptr) {
        settings_proxy_->deleteLater();
        settings_proxy_ = nullptr;
    }
}

const QString& WebWindow::appName() const
{
    return app_name_;
}

void WebWindow::setSearchManager(SearchManager *search_manager)
{
    search_manager_ = search_manager;

    connect(search_manager_, &SearchManager::searchContentResult,
            search_proxy_, &SearchProxy::onContentResult);
    connect(search_manager_, &SearchManager::searchContentMismatch,
            search_proxy_, &SearchProxy::mismatch);

    connect(search_manager_, &SearchManager::searchAnchorResult,
            this, &WebWindow::onSearchAnchorResult);
}

void WebWindow::setAppName(const QString &app_name)
{
    app_name_ = app_name;
    qDebug() << "appName:" << app_name << endl;
    if (0 == app_name.length()) {
        this->slot_ButtonHide();
    }
}

void WebWindow::setSearchKeyword(const QString &keyword)
{
    keyword_ = keyword;
}

void WebWindow::initConnections()
{
    connect(search_edit_, &SearchEdit::textChanged,
            this, &WebWindow::onSearchTextChanged);
    connect(search_edit_, &SearchEdit::downKeyPressed,
            completion_window_, &SearchCompletionWindow::goDown);
    connect(search_edit_, &SearchEdit::enterPressed,
            this, &WebWindow::onTitleBarEntered);
    connect(search_edit_, &SearchEdit::upKeyPressed,
            completion_window_, &SearchCompletionWindow::goUp);
    connect(search_edit_, &SearchEdit::focusChanged,
            this, &WebWindow::onSearchEditFocusOut);

    connect(completion_window_, &SearchCompletionWindow::resultClicked,
            this, &WebWindow::onSearchResultClicked);
    connect(completion_window_, &SearchCompletionWindow::searchButtonClicked,
            this, &WebWindow::onSearchButtonClicked);
    connect(&search_timer_, &QTimer::timeout,
            this, &WebWindow::onSearchTextChangedDelay);

    connect(this, &WebWindow::manualSearchByKeyword,
            this, &WebWindow::onManualSearchByKeyword);
}

void WebWindow::onManualSearchByKeyword(const QString &keyword)
{
    qDebug() << "WebWindow: onManualSearchByKeyword keyword:" << keyword;

    this->onSearchContentByKeyword(keyword);
}

void WebWindow::initUI()
{
    completion_window_ = new SearchCompletionWindow();
    completion_window_->hide();

    //init custom title
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(0);

    m_backButton = new DButtonBoxButton(DStyle::SP_ArrowLeave);
    m_backButton->setDisabled(true);
    m_backButton->setFixedSize(36, 36);

    m_forwardButton = new DButtonBoxButton(DStyle::SP_ArrowEnter);
    m_forwardButton->setDisabled(true);
    m_forwardButton->setFixedSize(36, 36);

    m_backButton->setShortcut(Qt::Key_Left);
    m_forwardButton->setShortcut(Qt::Key_Right);

    QList<DButtonBoxButton *> buttonList;
    buttonList << m_backButton << m_forwardButton;

    buttonBox = new Dtk::Widget::DButtonBox(this);
    buttonBox->setButtonList(buttonList, false);
    buttonBox->setFocusPolicy(Qt::NoFocus);

    buttonLayout->addWidget(buttonBox);
    buttonLayout->setSpacing(0);
    buttonLayout->setContentsMargins(13, 0, 0, 0);

    QFrame *buttonFrame = new QFrame(this);
    buttonFrame->setLayout(buttonLayout);

    search_edit_ = new SearchEdit(this);
    DFontSizeManager::instance()->bind(search_edit_, DFontSizeManager::T6, QFont::Normal);
    search_edit_->setObjectName("SearchEdit");
    search_edit_->setFixedSize(350, 44);
    search_edit_->setPlaceHolder(QObject::tr("Search"));

    this->titlebar()->addWidget(buttonFrame, Qt::AlignLeft);
    this->titlebar()->addWidget(search_edit_, Qt::AlignCenter);

    this->titlebar()->setSeparatorVisible(true);
    this->titlebar()->setIcon(QIcon::fromTheme("deepin-manual"));

    search_proxy_ = new SearchProxy(this);
    title_bar_proxy_ = new TitleBarProxy(this);
    connect(m_backButton, &DButtonBoxButton::clicked,
            title_bar_proxy_, &TitleBarProxy::backwardButtonClicked);
    connect(m_forwardButton, &DButtonBoxButton::clicked,
            title_bar_proxy_, &TitleBarProxy::forwardButtonClicked);
    connect(title_bar_proxy_, &TitleBarProxy::buttonShowSignal,
            this, &WebWindow::slot_ButtonShow);

    this->setFocusPolicy(Qt::ClickFocus);
}

void WebWindow::initWebView()
{
    image_viewer_ = new ImageViewer(this);
    image_viewer_proxy_ = new ImageViewerProxy(image_viewer_, this);
    manual_proxy_ = new ManualProxy(this);
    theme_proxy_ = new ThemeProxy(this);
    settings_proxy_ = new SettingsProxy(this);
    i18n_proxy = new I18nProxy(this);

    web_view_ = new ManualWebView();
    web_view_->setParentWindow(this);
    web_view_->page()->setEventDelegate(new WebEventDelegate(this));
    this->setCentralWidget(web_view_);

    // Disable web security.
    auto settings = web_view_->page()->settings();
    settings->setMinimumFontSize(8);
    settings->setWebSecurity(QCefWebSettings::StateDisabled);

    // init default font size
    settings->setDefaultFontSize(this->fontInfo().pixelSize());

    // Use TitleBarProxy instead.
    QWebChannel *web_channel = web_view_->page()->webChannel();
    web_view_->setAcceptDrops(false);

    web_channel->registerObject("i18n", i18n_proxy);
    web_channel->registerObject("imageViewer", image_viewer_proxy_);
    web_channel->registerObject("manual", manual_proxy_);
    web_channel->registerObject("search", search_proxy_);
    web_channel->registerObject("theme", theme_proxy_);
    web_channel->registerObject("titleBar", title_bar_proxy_);
    web_channel->registerObject("settings", settings_proxy_);

    connect(web_view_->page(), &QCefWebPage::loadFinished,
            this, &WebWindow::onWebPageLoadFinished);
    connect(manual_proxy_, &ManualProxy::WidgetLower, this, &WebWindow::lower);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            theme_proxy_, &ThemeProxy::slot_ThemeChange);
}

void WebWindow::initShortcuts()
{
    qDebug() << "init Short cuts" << endl;

    //设置窗口大小切换快捷键
    QShortcut *scWndReize = new QShortcut(this);
    scWndReize->setKey(tr("Ctrl+Alt+F"));
    scWndReize->setContext(Qt::WindowShortcut);
    scWndReize->setAutoRepeat(false);
    connect(scWndReize, &QShortcut::activated, this, [this]{
        if (this->windowState() & Qt::WindowMaximized) {
            this->showNormal();
        } else if (this->windowState() == Qt::WindowNoState){
            this->showMaximized();
        }
    });

    //设置搜索快捷键
    QShortcut *scSearch = new QShortcut(this);
    scSearch->setKey(tr("Ctrl+F"));
    scSearch->setContext(Qt::WindowShortcut);
    scSearch->setAutoRepeat(false);
    connect(scSearch, &QShortcut::activated, this, [this]{
        qDebug() << "search" << endl;
        search_edit_->lineEdit()->setFocus(Qt::ShortcutFocusReason);
        web_view_->page()->remapBrowserWindow(web_view_->winId(), this->winId());
    });
}

void WebWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    if (!is_index_loaded_) {

        emit this->shown(this);
        is_index_loaded_ = true;
        QTimer::singleShot(10, this, [this] {
            const QFileInfo info(kIndexPage);
            web_view_->load(QUrl::fromLocalFile(info.absoluteFilePath()));
        });
    }
}

void WebWindow::closeEvent(QCloseEvent *event)
{
    emit this->closed(app_name_);

    QWidget::closeEvent(event);
}

void WebWindow::onSearchContentByKeyword(const QString &keyword)
{
    qDebug() << "calling keyword is:" << keyword << endl;
    QString key(keyword);
    const QString searchKey = key.remove('\n').remove('\r').remove("\r\n");
    search_manager_->searchContent(searchKey);

    // Show search page.
    web_view_->page()->runJavaScript(QString("openSearchPage('%1')").arg(searchKey));
}

void WebWindow::onSearchEditFocusOut()
{
    QTimer::singleShot(20, [=]() {
        this->completion_window_->hide();
    });
}

void WebWindow::onSearchButtonClicked()
{
    QString text = search_edit_->text();

    this->onSearchContentByKeyword(text);

    completion_window_->hide();
}

void WebWindow::onSearchResultClicked(const SearchAnchorResult &result)
{
    web_view_->page()->runJavaScript(
        QString("open('%1', '%2', '%3', '%4')")
        .arg(result.app_name)
        .arg(result.anchorId)
        .arg(result.anchor)
        .arg(result.app_display_name));
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
    if (text.size() < 1 || text.toLower().contains(QRegExp("[+-_$!@#%^&\\(\\)]"))) {
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

    if (first_webpage_loaded_) {
        first_webpage_loaded_ = false;
        if (keyword_.length() > 0) {
            QTimer::singleShot(10, [=]{
                emit this->manualSearchByKeyword(keyword_);
            });
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
        completion_window_->setSearchAnchorResult(result);
        completion_window_->show();
        completion_window_->raise();
        completion_window_->autoResize();
        // Move to below of search edit.
        const QPoint local_point(this->rect().width() / 2 - search_edit_->width() / 2, titlebar()->height()-3);
        const QPoint global_point(this->mapToGlobal(local_point));
        completion_window_->move(global_point);
        completion_window_->setFocusPolicy(Qt::NoFocus);
        completion_window_->setFocusPolicy(Qt::StrongFocus);
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
                qDebug() << "eventFilter back";
                title_bar_proxy_->backwardButtonClicked();
                break;
            }
            case Qt::ForwardButton: {
                qDebug() << "eventFilter forward";
                title_bar_proxy_->forwardButtonClicked();
                break;
            }
            default: {
            }
        }
    }

    if (event->type() == QEvent::FontChange && watched == this) {
        if (this->settings_proxy_) {
            qDebug() << "eventFilter QEvent::FontChange";
            auto fontInfo = this->fontInfo();
            Q_EMIT this->settings_proxy_->fontChangeRequested(fontInfo.family(), fontInfo.pixelSize());
        }
    }

    return QObject::eventFilter(watched, event);
}

void WebWindow::slot_ButtonHide()
{
    QTimer::singleShot(20, [=]() {
        qDebug() << "slot_ButtonHide";
        buttonBox->hide();
    });
}

void WebWindow::slot_ButtonShow()
{
    QTimer::singleShot(20, [=]() {
        qDebug() << "slot_ButtonShow";
        buttonBox->show();
    });
}

}  // namespace dman

