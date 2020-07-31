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
#include "base/command.h"
#include "base/consts.h"
#include "base/utils.h"
#include "controller/search_manager.h"
#include "dbus/dbus_consts.h"
#include "view/i18n_proxy.h"
#include "view/image_viewer_proxy.h"
#include "view/manual_proxy.h"
#include "view/search_proxy.h"
#include "view/settings_proxy.h"
#include "view/theme_proxy.h"
#include "view/title_bar_proxy.h"
#include "view/widget/image_viewer.h"
#include "view/widget/search_completion_window.h"
#include "view/widget/search_edit.h"
#include "controller/config_manager.h"

#include <QApplication>
#include <QDBusConnection>
#include <QFileInfo>
#include <QMouseEvent>
#include <QShortcut>
#include <QShowEvent>
#include <QWebChannel>
#include <QWindow>
#include <QX11Info>

#include <DApplicationHelper>
#include <DButtonBox>
#include <DLog>
#include <DPlatformWindowHandle>
#include <DTitlebar>
#include <QStyleOptionViewItem>

DWIDGET_USE_NAMESPACE

namespace dman {

namespace {

const int kSearchDelay = 200;
static constexpr const char *CONFIG_WINDOW_WIDTH = "window_width";
static constexpr const char *CONFIG_WINDOW_HEIGHT = "window_height";
static constexpr const char *CONFIG_WINDOW_INFO = "window_info";

}  // namespace

WebWindow::WebWindow(QWidget *parent)
    : Dtk::Widget::DMainWindow(parent)
    , app_name_()
    , title_name_("")
    , search_timer_()
    , keyword_("")
    , first_webpage_loaded_(true)
{
    // 使用 redirectContent 模式，用于内嵌 x11 窗口时能有正确的圆角效果
    Dtk::Widget::DPlatformWindowHandle::enableDXcbForWindow(this, true);

    search_timer_.setSingleShot(true);
    setAttribute(Qt::WA_InputMethodEnabled, true);

    this->initUI();
    this->initConnections();
    this->initShortcuts();
    this->initDBus();

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

const QString &WebWindow::appName() const
{
    return app_name_;
}

void WebWindow::setSearchManager(SearchManager *search_manager)
{
    search_manager_ = search_manager;

    connect(search_manager_, &SearchManager::searchContentResult, search_proxy_,
            &SearchProxy::onContentResult);
    connect(search_manager_, &SearchManager::searchContentMismatch, search_proxy_,
            &SearchProxy::mismatch);

    connect(search_manager_, &SearchManager::searchAnchorResult, this,
            &WebWindow::onSearchAnchorResult);
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

/**
 * @brief WebWindow::initConnections
 */
void WebWindow::initConnections()
{
    connect(search_edit_, &SearchEdit::textChanged, this, &WebWindow::onSearchTextChanged);
    connect(search_edit_, &SearchEdit::downKeyPressed, completion_window_,
            &SearchCompletionWindow::goDown);
    connect(search_edit_, &SearchEdit::enterPressed, this, &WebWindow::onTitleBarEntered);
    connect(search_edit_, &SearchEdit::upKeyPressed, completion_window_,
            &SearchCompletionWindow::goUp);
    connect(search_edit_, &SearchEdit::focusChanged, this, &WebWindow::onSearchEditFocusOut);

    connect(completion_window_, &SearchCompletionWindow::resultClicked, this,
            &WebWindow::onSearchResultClicked);
    connect(completion_window_, &SearchCompletionWindow::searchButtonClicked, this,
            &WebWindow::onSearchButtonClicked);
    connect(&search_timer_, &QTimer::timeout, this, &WebWindow::onSearchTextChangedDelay);

    connect(this, &WebWindow::manualSearchByKeyword, this, &WebWindow::onManualSearchByKeyword);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &WebWindow::onThemeChange);

}

void WebWindow::onManualSearchByKeyword(const QString &keyword)
{
    qDebug() << "WebWindow: onManualSearchByKeyword keyword:" << keyword;

    this->onSearchContentByKeyword(keyword);
}

/**
 * @brief WebWindow::onACtiveColorChanged
 * @param map 个性化对, QString:更改类型  QVariant:更改后的值
 * @note 改变系统个性化时，触发此槽
 */
void WebWindow::onACtiveColorChanged(QString, QMap<QString, QVariant>map, QStringList)
{
    QString strValue = map.begin().value().toString();
    QString strKey = map.begin().key();
    qDebug() << __func__ << " key: " << strKey << " value: " << strValue;
    if (0 == strKey.compare("QtActiveColor")) {
        QTimer::singleShot(100, this, [&]() {
            //获取系统活动色
            QColor fillColor = DGuiApplicationHelper::instance()->applicationPalette().highlight().color();
            web_view_->page()->runJavaScript(QString("setHashWordColor('%1')").arg(fillColor.name()));
        });
    } else if (0 == strKey.compare("StandardFont")) {
        //获取系统字体
        web_view_->page()->runJavaScript(QString("setWordFontfamily('%1')").arg(strValue));
    }
}

void WebWindow::onThemeChange(DGuiApplicationHelper::ColorType themeType)
{
    Q_UNUSED(themeType)
    if (web_view_) {
        QColor fillColor = DGuiApplicationHelper::instance()->applicationPalette().highlight().color();
        web_view_->page()->runJavaScript(QString("setHashWordColor('%1')").arg(fillColor.name()));
    } else {
        QTimer::singleShot(300, this, [&]() {
            QColor fillColor = DGuiApplicationHelper::instance()->applicationPalette().highlight().color();
            web_view_->page()->runJavaScript(QString("setHashWordColor('%1')").arg(fillColor.name()));
        });
    }
}

/**
 * @brief WebWindow::initUI
 * @note 初始化窗口
 */

void WebWindow::initUI()
{
    completion_window_ = new SearchCompletionWindow();
    completion_window_->hide();

    // 初始化标题栏
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(0);

    m_backButton = new DButtonBoxButton(DStyle::SP_ArrowLeave);
    m_backButton->setDisabled(true);
    m_backButton->setFixedSize(36, 36);
    m_backButton->setEnabled(false);

    m_forwardButton = new DButtonBoxButton(DStyle::SP_ArrowEnter);
    m_forwardButton->setDisabled(true);
    m_forwardButton->setFixedSize(36, 36);
    m_forwardButton->setEnabled(false);

    m_backButton->setShortcut(Qt::Key_Left);
    m_forwardButton->setShortcut(Qt::Key_Right);

    QList<DButtonBoxButton *> buttonList;
    buttonList << m_backButton << m_forwardButton;

    buttonBox = new Dtk::Widget::DButtonBox(this);
    buttonBox->setButtonList(buttonList, false);
    buttonBox->setFocusPolicy(Qt::NoFocus);
    updateBtnBox();

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
    search_edit_->setFocus();


    this->titlebar()->addWidget(buttonFrame, Qt::AlignLeft);
    this->titlebar()->addWidget(search_edit_, Qt::AlignCenter);

    this->titlebar()->setSeparatorVisible(true);
    this->titlebar()->setIcon(QIcon::fromTheme("deepin-manual"));
    this->titlebar()->setFocusPolicy(Qt::StrongFocus);

    search_proxy_ = new SearchProxy(this);
    title_bar_proxy_ = new TitleBarProxy(this);
    connect(m_backButton, &DButtonBoxButton::clicked, title_bar_proxy_,
            &TitleBarProxy::backwardButtonClicked);
    connect(m_forwardButton, &DButtonBoxButton::clicked, title_bar_proxy_,
            &TitleBarProxy::forwardButtonClicked);
    connect(title_bar_proxy_, &TitleBarProxy::buttonShowSignal, this, &WebWindow::slot_ButtonShow);

    //获取窗口上次保存尺寸,加载上次保存尺寸.
    QSettings *setting = ConfigManager::getInstance()->getSettings();
    setting->beginGroup(CONFIG_WINDOW_INFO);
    int saveWidth = setting->value(CONFIG_WINDOW_WIDTH).toInt();
    int saveHeight = setting->value(CONFIG_WINDOW_HEIGHT).toInt();
    setting->endGroup();
    qDebug() << "load window_width: " << saveWidth;
    qDebug() << "load window_height: " << saveHeight;
    // 如果配置文件没有数据
    if (saveWidth == 0 || saveHeight == 0) {
        saveWidth = 1024;
        saveHeight = 680;
    }

    //隐藏title阴影
    this->setTitlebarShadowEnabled(false);
    resize(QSize(saveWidth, saveHeight));


    this->setFocusPolicy(Qt::ClickFocus);
}

/**
 * @brief WebWindow::initWebView
 * @note 初始化qwebengines, 初始化QWebChannel,通过QWebChannel完成QT与JS间通讯
 */
void WebWindow::initWebView()
{
    image_viewer_ = new ImageViewer(this);
    image_viewer_proxy_ = new ImageViewerProxy(image_viewer_, this);
    theme_proxy_ = new ThemeProxy(this);
    settings_proxy_ = new SettingsProxy(this);
    i18n_proxy = new I18nProxy(this);
    manual_proxy_ = new ManualProxy(this);
    //将当前存在的applist传入到数据库管理类中.
    if (search_manager_) {
        QStringList strlist = manual_proxy_->getSystemManualList();
        emit search_manager_->installApps(strlist);
    }

    web_view_ = new QWebEngineView;
    this->setCentralWidget(web_view_);
    web_view_->hide();
    web_view_->setAcceptDrops(false);
    slot_ThemeChanged();
    QWebChannel *web_channel = new QWebChannel;
    web_channel->registerObject("i18n", i18n_proxy);
    web_channel->registerObject("imageViewer", image_viewer_proxy_);
    web_channel->registerObject("manual", manual_proxy_);
    web_channel->registerObject("search", search_proxy_);
    web_channel->registerObject("theme", theme_proxy_);
    web_channel->registerObject("titleBar", title_bar_proxy_);
    web_channel->registerObject("settings", settings_proxy_);
    web_view_->page()->setWebChannel(web_channel);

    connect(web_view_->page(), &QWebEnginePage::loadFinished, this, &WebWindow::onWebPageLoadFinished);
    connect(manual_proxy_, &ManualProxy::WidgetLower, this, &WebWindow::lower);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            theme_proxy_, &ThemeProxy::slot_ThemeChange);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            this, &WebWindow::slot_ThemeChanged);
}

void WebWindow::setTitleName(const QString &title_name)
{
    title_name_ = title_name;
}

/**
 * @brief WebWindow::cancelTextChanged
 * @note 取消文本选中状态，用于在帮助手册首页时不显示＂Ｃopy＂按钮
 * 每次切换页面时取消显示menu，当selectionChanged触发时设置显示menu
 */
void WebWindow::cancelTextChanged()
{
    web_view_->setContextMenuPolicy(Qt::NoContextMenu);
}

/**
 * @brief WebWindow::updateBtnBox 更新当前按钮组状态
 */
void WebWindow::updateBtnBox()
{
    if (m_forwardButton->isEnabled() || m_backButton->isEnabled()) {
        buttonBox->show();
    } else {
        buttonBox->hide();
    }
}

/**
 * @brief WebWindow::saveWindowSize 记录最后一个窗口关闭时的大小
 */
void WebWindow::saveWindowSize()
{
    QSettings *setting = ConfigManager::getInstance()->getSettings();
    setting->beginGroup(CONFIG_WINDOW_INFO);
    setting->setValue(CONFIG_WINDOW_WIDTH, width());
    setting->setValue(CONFIG_WINDOW_HEIGHT, height());
    setting->endGroup();
    qDebug() << __func__ << "save window_Size:" << width() << ", " << height();
}

/**
 * @brief WebWindow::initShortcuts 设置窗口支持的快捷键
 */
void WebWindow::initShortcuts()
{
    qDebug() << "init Short cuts" << endl;
    //设置窗口大小切换快捷键
    QShortcut *scWndReize = new QShortcut(this);
    scWndReize->setKey(tr("Ctrl+Alt+F"));
    scWndReize->setContext(Qt::WindowShortcut);
    scWndReize->setAutoRepeat(false);
    connect(scWndReize, &QShortcut::activated, this, [this] {
        if (this->windowState() & Qt::WindowMaximized)
        {
            this->showNormal();
        } else if (this->windowState() == Qt::WindowNoState)
        {
            this->showMaximized();
        }
    });

    //设置搜索快捷键  后期将支持盲打功能,故不需要此快捷键
    QShortcut *scSearch = new QShortcut(this);
    scSearch->setKey(tr("Ctrl+F"));
    scSearch->setContext(Qt::WindowShortcut);
    scSearch->setAutoRepeat(false);
    connect(scSearch, &QShortcut::activated, this, [this] {
        qDebug() << "search" << endl;
        search_edit_->lineEdit()->setFocus(Qt::ShortcutFocusReason);
    });
}

/**
 * @brief WebWindow::initDBus 初始化Dbus接口, 获取系统设置中个性化设置变化信号.
 */
void WebWindow::initDBus()
{
    QDBusConnection senderConn = QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");
    if (!senderConn.connect(
                "com.deepin.daemon.Appearance",  // sender's service name
                "/com/deepin/daemon/Appearance",    // sender's path name
                "org.freedesktop.DBus.Properties",  // interface
                "PropertiesChanged",                                   // sender's signal name
                this,                                           // receiver
                SLOT(onACtiveColorChanged(QString, QMap<QString, QVariant>, QStringList)))) {

        qDebug() << "connectToBus()::connect()  PropertiesChanged failed";
    } else {
        qDebug() << "connectToBus()::connect()  PropertiesChanged success";
    }
}

/**
 * @brief WebWindow::setHashWordColor 调用ｊｓ接口，改变活动色
 */
void WebWindow::setHashWordColor()
{
    QColor Color = DGuiApplicationHelper::instance()->applicationPalette().highlight().color();
    QString strColor = Color.name(QColor::NameFormat::HexRgb);
    web_view_->page()->runJavaScript(QString("setHashWordColor('%1')").arg(strColor));
    completion_window_->updateColor(Color);
}

/**
 * @brief WebWindow::settingContextMenu 初始化鼠标选中内容右键菜单Menu.
 */
void WebWindow::settingContextMenu()
{
    web_view_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(web_view_, &QWebEngineView::selectionChanged, this, [this]() {
        web_view_->setContextMenuPolicy(Qt::CustomContextMenu);
    });
    QMenu *menu = new QMenu(this);
    QAction *action =  menu->addAction(QObject::tr("Copy"));
    connect(web_view_, &QWidget::customContextMenuRequested, this, [ = ]() {
        if (!web_view_->selectedText().isEmpty()) {
            menu->exec(QCursor::pos());
        }
    });
    connect(action, &QAction::triggered, this, [ = ]() {
        QApplication::clipboard()->setText(web_view_->selectedText());
    });
}

void WebWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    if (!is_index_loaded_) {
        is_index_loaded_ = true;
        //延迟显示webView，200ms主要用于web_view_->load();
        QTimer::singleShot(200, this, [this] {
            qDebug() << Q_FUNC_INFO;
            emit this->shown(this);
            this->initWebView();
            const QFileInfo info(kIndexPage);
            web_view_->load(QUrl::fromLocalFile(info.absoluteFilePath()));
        });
    }
}
//void WebWindow::inputMethodEvent(QInputMethodEvent *e)
//{
//    if (!e->commitString().isEmpty()) {
//        search_edit_->lineEdit()->setText(e->commitString());
//        search_edit_->lineEdit()->setFocus();
//    }
//    QWidget::inputMethodEvent(e);
//}

void WebWindow::closeEvent(QCloseEvent *event)
{
    emit this->closed(app_name_);
    //保存窗口大小
    saveWindowSize();

    QWidget::closeEvent(event);
}

/**
 * @brief WebWindow::onSearchContentByKeyword
 * @param keyword 搜索关键字
 * @note 通过关键字进行搜索，显示探索页面
 */
void WebWindow::onSearchContentByKeyword(const QString &keyword)
{
    qDebug() << "calling keyword is:" << keyword << endl;
    QString key(keyword);
    const QString searchKey = key.remove('\n').remove('\r').remove("\r\n");
    //发送信号，通过Ｓearch_db类执行搜索
    emit search_manager_->searchContent(searchKey);

    QString base64Key = QString(searchKey.toUtf8().toBase64());
    qDebug() << base64Key << endl;

    // 调用ｊｓ接口显示搜索内容
    web_view_->page()->runJavaScript(QString("openSearchPage('%1')").arg(base64Key));
}

/**
 * @brief WebWindow::onSearchEditFocusOut 搜索框失去焦点
 */
void WebWindow::onSearchEditFocusOut()
{
    QTimer::singleShot(20, this, [ = ]() {
        this->completion_window_->hide();
    });
}

/**
 * @brief WebWindow::onSearchButtonClicked
 * @note 输入搜索关键字之后点击弹出窗口中的最下部的 SearchButton触发
 */
void WebWindow::onSearchButtonClicked()
{
    QString text = search_edit_->text();

    this->onSearchContentByKeyword(text);

    completion_window_->hide();
}

/**
 * @brief WebWindow::onSearchResultClicked
 * @param result
 * @note 点击SearchList Item 时调用js方法
 */
void WebWindow::onSearchResultClicked(const SearchAnchorResult &result)
{
    web_view_->page()->runJavaScript(QString("open('%1', '%2', '%3', '%4')")
                                     .arg(result.app_name
                                          , result.anchorId
                                          , result.anchor
                                          , result.app_display_name));

}

/**
 * @brief WebWindow::onSearchTextChanged
 * @param text
 * @note ２００ｍｓ内搜索框中的文本改变时重新计时执行一次搜索操作, 200ms搜索一次
 */
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
    const QString text = textTemp.remove('\n').remove('\r').remove("\r\n").remove(QRegExp("\\s"));
    // 过滤特殊字符
    if (text.isEmpty() || text.contains(QRegExp("[+-_$!@#%^&\\(\\)]"))) {
        return;
    }
    completion_window_->setKeyword(text);
    //执行搜索
    emit search_manager_->searchAnchor(text);
}

void WebWindow::onTitleBarEntered()
{
    QString textTemp = search_edit_->text();
    const QString text = textTemp.remove('\n').remove('\r').remove("\r\n").remove(QRegExp("\\s"));
    if (text.size() >= 1) {
        completion_window_->onEnterPressed();
    }
}

/**
 * @brief WebWindow::onWebPageLoadFinished qwebengines加载页面完成槽
 * @note  网页加载完成后设置其配置
 * @param ok
 */
void WebWindow::onWebPageLoadFinished(bool ok)
{
    //改变ｊs颜色
    setHashWordColor();
    settingContextMenu();
    qDebug() << Q_FUNC_INFO << " onWebPageLoadFinished :" << ok;
    if (ok) {
        QString qsthemetype = "Null";
        DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
        if (themeType == DGuiApplicationHelper::LightType) {
            qsthemetype = "LightType";
        } else if (themeType == DGuiApplicationHelper::DarkType) {
            qsthemetype = "DarkType";
        }
        web_view_->page()->runJavaScript(QString("setTheme('%1')").arg(qsthemetype));
        if (app_name_.isEmpty()) {
            web_view_->page()->runJavaScript("index()");
        } else {
            QString real_path(app_name_);
            if (real_path.contains('/')) {
                // Open markdown file with absolute path.
                QFileInfo info(real_path);
                real_path = info.canonicalFilePath();
                web_view_->page()->runJavaScript(QString("open('%1')").arg(real_path));
            } else {
                // Open system manual.
                web_view_->page()->runJavaScript(QString("open('%1')").arg(app_name_));
            }

            if (!title_name_.isEmpty()) {
                web_view_->page()->runJavaScript(QString("linkTitle('%1')").arg(title_name_));
            }
        }

        QTimer::singleShot(100, this, [&]() {
            web_view_->show();
            if (first_webpage_loaded_) {
                first_webpage_loaded_ = false;
                if (keyword_.length() > 0) {
                    qDebug() << "first_webpage_loaded_ manualSearchByKeyword:" << keyword_;
                    emit this->manualSearchByKeyword(keyword_);
                }
            }

            if (this->settings_proxy_) {
                auto fontInfo = this->fontInfo();
                emit this->settings_proxy_->fontChangeRequested(fontInfo.family(),
                                                                fontInfo.pixelSize());
            }
        });
    }
}
/**
 * @brief WebWindow::onSearchAnchorResult 搜索结果框内容显示
 * @param keyword 搜索关键字
 * @param result  搜索的内容
 */
void WebWindow::onSearchAnchorResult(const QString &keyword, const SearchAnchorResultList &result)
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
        const QPoint local_point(this->rect().width() / 2 - search_edit_->width() / 2,
                                 titlebar()->height() - 3);
        const QPoint global_point(this->mapToGlobal(local_point));
        completion_window_->move(global_point);
        completion_window_->setFocusPolicy(Qt::NoFocus);
        completion_window_->setFocusPolicy(Qt::StrongFocus);
    }
}

void WebWindow::inputMethodEvent(QInputMethodEvent *e)
{
    if (!e->commitString().isEmpty()) {
        search_edit_->lineEdit()->setText(e->commitString());
        search_edit_->lineEdit()->setFocus();
    }

    QWidget::inputMethodEvent(e);
}

QVariant WebWindow::inputMethodQuery(Qt::InputMethodQuery prop) const
{
    switch (prop) {
    case Qt::ImEnabled:
        return true;
    case Qt::ImCursorRectangle:
    default:
        ;
    }

    return QWidget::inputMethodQuery(prop);
}

bool WebWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease && qApp->activeWindow() == this
            && watched->objectName() == QLatin1String("QMainWindowClassWindow")) {
        if (web_view_ != nullptr) {
            if (web_view_->selectedText().isEmpty()) {
                this->setFocus();
            }
        }
    }
    if (event->type() == QEvent::KeyPress && qApp->activeWindow() == this
            && watched->objectName() == QLatin1String("QMainWindowClassWindow")) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_V
                && keyEvent->modifiers().testFlag(Qt::ControlModifier)) {
            const QString &clipboardText = QApplication::clipboard()->text();
            // support Ctrl+V shortcuts.
            if (!clipboardText.isEmpty()) {
                search_edit_->lineEdit()->setFocus();
            }
        } else if (keyEvent->key() == Qt::Key_C
                   && keyEvent->modifiers().testFlag(Qt::ControlModifier)) {
            QApplication::clipboard()->setText(web_view_->selectedText());
        } else if (((keyEvent->key() <= Qt::Key_Z && keyEvent->key() >= Qt::Key_A)
                    || (keyEvent->key() <= Qt::Key_9 && keyEvent->key() >= Qt::Key_0)
                    || (keyEvent->key() == Qt::Key_Space))
                   && keyEvent->modifiers() == Qt::NoModifier) {
            search_edit_->lineEdit()->setFocus();
        }
    }
    // Filters mouse press event only.
    if (event->type() == QEvent::MouseButtonPress && qApp->activeWindow() == this &&
            watched->objectName() == QLatin1String("QMainWindowClassWindow")) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button()) {
            switch (mouseEvent->button()) {
            case Qt::BackButton: {
                qDebug() << "eventFilter back";
                emit title_bar_proxy_->backwardButtonClicked();
                break;
            }
            case Qt::ForwardButton: {
                qDebug() << "eventFilter forward";
                emit title_bar_proxy_->forwardButtonClicked();
                break;
            }
            case Qt::MiddleButton: {
                return true;
            }
            default: {
            }
            if (!search_edit_->geometry().contains(this->mapFromGlobal(QCursor::pos()))) {
                completion_window_->hide();
            }
            }
        }
    }

    //过滤字体改变事件
    if (event->type() == QEvent::FontChange && watched == this) {
        if (this->settings_proxy_) {
            qDebug() << "eventFilter QEvent::FontChange";
            auto fontInfo = this->fontInfo();
            emit this->settings_proxy_->fontChangeRequested(fontInfo.family(),
                                                            fontInfo.pixelSize());
        }
    }

    return QObject::eventFilter(watched, event);
}

/**
 * @brief WebWindow::slot_ButtonHide
 * @note 隐藏前进/后退按钮
 */
void WebWindow::slot_ButtonHide()
{
//    QTimer::singleShot(20, [ = ]() {
//        qDebug() << "slot_ButtonHide";
//        buttonBox->hide();
//    });
    buttonBox->hide();
}

/**
 * @brief WebWindow::slot_ButtonShow
 * @note 显示前进/后退按钮
 */
void WebWindow::slot_ButtonShow()
{
    QTimer::singleShot(20, this, [ = ]() {
        qDebug() << "slot_ButtonShow";
        buttonBox->show();
    });
}
/**
 * @brief WebWindow::slot_ThemeChanged
 * @note 修改背景颜色，　减少WebEngeineView 控件在黑色主题下的明显闪屏效果
 */
void WebWindow::slot_ThemeChanged()
{
    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    if (themeType == DGuiApplicationHelper::LightType)
        web_view_->page()->setBackgroundColor(QColor(0xF8, 0xF8, 0xF8));
    else if (themeType == DGuiApplicationHelper::DarkType) {
        web_view_->page()->setBackgroundColor(QColor(0x28, 0x28, 0x28));
    }
}
}  // namespace dman
