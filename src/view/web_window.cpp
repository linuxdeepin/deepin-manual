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

DWIDGET_USE_NAMESPACE

namespace dman {

namespace {

const int kSearchDelay = 200;

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
}

void WebWindow::onManualSearchByKeyword(const QString &keyword)
{
    qDebug() << "WebWindow: onManualSearchByKeyword keyword:" << keyword;

    this->onSearchContentByKeyword(keyword);
}

/**
 * @brief WebWindow::onACtiveColorChanged
 * @param map
 * 改变系统活动用色时，触发此槽，
 */
void WebWindow::onACtiveColorChanged(QString, QMap<QString, QVariant>map, QStringList)
{

    QString strValue = map.begin().value().toString();
    QString strKey = map.begin().key();
    if (0 == strKey.compare("QtActiveColor")) {
        web_view_->page()->runJavaScript(QString("setHashWordColor('%1')").arg(strValue));
        completion_window_->updateColor(QColor(strValue));
    } else if (0 == strKey.compare("StandardFont")) {
        web_view_->page()->runJavaScript(QString("setWordFontfamily('%1')").arg(strValue));
    }
}

void WebWindow::initUI()
{
    completion_window_ = new SearchCompletionWindow();
    completion_window_->hide();

    // init custom title
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

    search_edit_ = new SearchEdit;
    DFontSizeManager::instance()->bind(search_edit_, DFontSizeManager::T6, QFont::Normal);
    search_edit_->setObjectName("SearchEdit");
    search_edit_->setFixedSize(350, 44);
    search_edit_->setPlaceHolder(QObject::tr("Search"));

    DMenu *pMenu = new DMenu;
    QAction *pHelpSupport = new QAction(tr("UOS Support"));
    pMenu->addAction(pHelpSupport);
    this->titlebar()->setMenu(pMenu);


    this->titlebar()->addWidget(buttonFrame, Qt::AlignLeft);
    this->titlebar()->addWidget(search_edit_, Qt::AlignCenter);
    this->titlebar()->setSeparatorVisible(true);
    this->titlebar()->setIcon(QIcon::fromTheme("deepin-manual"));
    //隐藏title阴影
    this->setTitlebarShadowEnabled(false);

    search_proxy_ = new SearchProxy(this);
    title_bar_proxy_ = new TitleBarProxy(this);
    connect(m_backButton, &DButtonBoxButton::clicked, title_bar_proxy_,
            &TitleBarProxy::backwardButtonClicked);
    connect(m_forwardButton, &DButtonBoxButton::clicked, title_bar_proxy_,
            &TitleBarProxy::forwardButtonClicked);
    connect(title_bar_proxy_, &TitleBarProxy::buttonShowSignal, this, &WebWindow::slot_ButtonShow);
    connect(pHelpSupport, &QAction::triggered, this, &WebWindow::slot_HelpSupportTriggered);

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
    resize(QSize(saveWidth, saveHeight));

    search_edit_->setFocus();

    this->setFocusPolicy(Qt::ClickFocus);
}

void WebWindow::initWebView()
{
    image_viewer_ = new ImageViewer(this);
    image_viewer_proxy_ = new ImageViewerProxy(image_viewer_, this);
    theme_proxy_ = new ThemeProxy(this);
    settings_proxy_ = new SettingsProxy(this);
    i18n_proxy = new I18nProxy(this);
    manual_proxy_ = new ManualProxy(this);
    // Do real search.
    if (search_manager_) {
        QStringList strlist = manual_proxy_->getSystemManualList();
        search_manager_->installApps(strlist);
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
    connect(manual_proxy_, &ManualProxy::channelInit, this, &WebWindow::onChannelFinish);
    connect(manual_proxy_, &ManualProxy::WidgetLower, this, &WebWindow::lower);
    connect(search_proxy_, &SearchProxy::setKeyword, this, &WebWindow::onSetKeyword);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            theme_proxy_, &ThemeProxy::slot_ThemeChange);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            this, &WebWindow::slot_ThemeChanged);

    manual_proxy_->setApplicationState("dde");
}

void WebWindow::setTitleName(const QString &title_name)
{
    title_name_ = title_name;
}

void WebWindow::cancelTextChanged()
{
    web_view_->setContextMenuPolicy(Qt::NoContextMenu);
}

void WebWindow::updateBtnBox()
{
    if (m_forwardButton->isEnabled() || m_backButton->isEnabled()) {
        buttonBox->show();
    } else {
        buttonBox->hide();
    }
}

void WebWindow::openjsPage(const QString &app_name, const QString &title_name)
{
    if (app_name.isEmpty()) {
        web_view_->page()->runJavaScript("index()");
    } else {
        QString real_path(app_name);
        if (real_path.contains('/')) {
            // Open markdown file with absolute path.
            QFileInfo info(real_path);
            real_path = info.canonicalFilePath();
            web_view_->page()->runJavaScript(QString("open('%1')").arg(real_path));
        } else {
            // Open system manual.
//            web_view_->page()->runJavaScript(QString("open('%1')").arg(app_name));
            web_view_->page()->runJavaScript(QString("openTitle('%1','%2')").arg(app_name, title_name));
        }

//        if (!title_name.isEmpty()) {
//            web_view_->page()->runJavaScript(QString("linkTitle('%1')").arg(title_name));
//        }
    }
}

void WebWindow::saveWindowSize()
{
    // 记录最后一个正常窗口的大小
    QSettings *setting = ConfigManager::getInstance()->getSettings();
    setting->beginGroup(CONFIG_WINDOW_INFO);
    setting->setValue(CONFIG_WINDOW_WIDTH, width());
    setting->setValue(CONFIG_WINDOW_HEIGHT, height());
    setting->endGroup();
    qDebug() << "save window_Size:" << width() << ", " << height();
}

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

    //设置搜索快捷键
    QShortcut *scSearch = new QShortcut(this);
    scSearch->setKey(tr("Ctrl+F"));
    scSearch->setContext(Qt::WindowShortcut);
    scSearch->setAutoRepeat(false);
    connect(scSearch, &QShortcut::activated, this, [this] {
        qDebug() << "search" << endl;
        search_edit_->lineEdit()->setFocus(Qt::ShortcutFocusReason);
//        web_view_->page()->remapBrowserWindow(web_view_->winId(), this->winId());
    });
}

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

void WebWindow::setHashWordColor()
{
    QColor Color = DGuiApplicationHelper::instance()->applicationPalette().highlight().color();
    QString strColor = Color.name(QColor::NameFormat::HexRgb);
    web_view_->page()->runJavaScript(QString("setHashWordColor('%1')").arg(strColor));
    completion_window_->updateColor(Color);
}

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
        QTimer::singleShot(200, this, [this] {
            qDebug() << Q_FUNC_INFO;
            emit this->shown(this);
            this->initWebView();
            const QFileInfo info(kIndexPage);
            qDebug() << Q_FUNC_INFO << "web_View_->load ... ";
            web_view_->load(QUrl::fromLocalFile(info.absoluteFilePath()));
        });
    }
}

void WebWindow::closeEvent(QCloseEvent *event)
{
    emit this->closed(app_name_);
    saveWindowSize();

    QWidget::closeEvent(event);
}

void WebWindow::onSearchContentByKeyword(const QString &keyword)
{
    qDebug() << "calling keyword is:" << keyword << endl;
    QString key(keyword);
    const QString searchKey = key.remove('\n').remove('\r').remove("\r\n").remove(QRegExp("\\s"));
    search_manager_->searchContent(searchKey);

    QString base64Key = QString(searchKey.toUtf8().toBase64());
    qDebug() << " base64Key " << base64Key << endl;

    // Show search page.
    web_view_->page()->runJavaScript(QString("openSearchPage('%1')").arg(base64Key));
}

void WebWindow::onSearchEditFocusOut()
{
    QTimer::singleShot(20, [ = ]() {
        this->completion_window_->hide();
    });
}

void WebWindow::onSearchButtonClicked()
{
    QString textTemp = search_edit_->text();
    const QString text = textTemp.remove('\n').remove('\r').remove("\r\n");
    this->onSearchContentByKeyword(text);
    completion_window_->hide();
}

void WebWindow::onSearchResultClicked(const SearchAnchorResult &result)
{
    web_view_->page()->runJavaScript(QString("open('%1', '%2', '%3', '%4')")
                                     .arg(result.app_name)
                                     .arg(result.anchorId)
                                     .arg(result.anchor)
                                     .arg(result.app_display_name));
}

void WebWindow::onSearchTextChanged(const QString &text)
{
    if (bIsSetKeyword) {
        bIsSetKeyword = false;
    } else if (text.size() >= 1) {
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
//    else if (textTemp.isEmpty()) {
//        m_backButton->click();
//    }
}


void WebWindow::onWebPageLoadFinished(bool ok)
{
    Q_UNUSED(ok)
    /*
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
           qDebug() << Q_FUNC_INFO << " app:" << app_name_ << " title:" << title_name_;
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
    //                web_view_->page()->runJavaScript(QString("open('%1')").arg(app_name_));
                   web_view_->page()->runJavaScript(QString("openTitle('%1','%2')").arg(app_name_, title_name_));
               }

    //            if (!title_name_.isEmpty()) {
    //                web_view_->page()->runJavaScript(QString("linkTitle('%1')").arg(title_name_));
    //            }
           }

           QTimer::singleShot(100, [&]() {
               qDebug() << "show webview";
               qDebug() << Q_FUNC_INFO << 481;
               web_view_->show();
               qDebug() << Q_FUNC_INFO << 482;
               if (first_webpage_loaded_) {
                   first_webpage_loaded_ = false;
                   qDebug() << Q_FUNC_INFO << 486;
                   if (keyword_.length() > 0) {
                       qDebug() << "first_webpage_loaded_ manualSearchByKeyword:" << keyword_;
                       emit this->manualSearchByKeyword(keyword_);
                   }
               }

               if (this->settings_proxy_) {
                   qDebug() << Q_FUNC_INFO << 494;
                   auto fontInfo = this->fontInfo();
                   Q_EMIT this->settings_proxy_->fontChangeRequested(fontInfo.family(),
                                                                     fontInfo.pixelSize());
               }
           });
       }
       */
}

/**
 * @brief WebWindow::onChannelFinish  完成channel对象与Qt对象绑定后回调
 */
void WebWindow::onChannelFinish()
{
    qDebug() << Q_FUNC_INFO;

    setHashWordColor();
    settingContextMenu();
    //设置主题
    QString qsthemetype = "Null";
    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    if (themeType == DGuiApplicationHelper::LightType) {
        qsthemetype = "LightType";
    } else if (themeType == DGuiApplicationHelper::DarkType) {
        qsthemetype = "DarkType";
    }
    web_view_->page()->runJavaScript(QString("setTheme('%1')").arg(qsthemetype));

    //设置打开页面
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
            web_view_->page()->runJavaScript(QString("openTitle('%1','%2')").arg(app_name_, title_name_));
        }
    }
    if (first_webpage_loaded_) {
        first_webpage_loaded_ = false;
        if (keyword_.length() > 0) {
            emit this->manualSearchByKeyword(keyword_);
        }
    }
    //设置字体
    if (this->settings_proxy_) {
        QFontInfo fontInfo = this->fontInfo();
        emit this->settings_proxy_->fontChangeRequested(fontInfo.family(),
                                                        fontInfo.pixelSize());
    }
    web_view_->show();
}

/**
 * @brief WebWindow::onSetKeyword JS根据页面关键字回调设置搜索框
 * @param keyword 关键字
 */
void WebWindow::onSetKeyword(const QString &keyword)
{
    bIsSetKeyword = true;
    QTimer::singleShot(40, [ = ]() {
        bIsSetKeyword = false;
    });

    if (search_edit_) {
        if (keyword.isEmpty()) {
            search_edit_->clearEdit();
        } else {
            search_edit_->setText(keyword);
        }
    }
}

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

    if (event->type() == QEvent::MouseButtonRelease && qApp->activeWindow() == this &&
            watched->objectName() == QLatin1String("QMainWindowClassWindow")) {
        this->setFocus();
    }

    if (event->type() == QEvent::KeyPress && qApp->activeWindow() == this &&
            watched->objectName() == QLatin1String("QMainWindowClassWindow")) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_V &&
                keyEvent->modifiers().testFlag(Qt::ControlModifier)) {
            const QString &clipboardText = QApplication::clipboard()->text();
            // support Ctrl+V shortcuts.
            if (!clipboardText.isEmpty()) {
                search_edit_->lineEdit()->setText(clipboardText);
                search_edit_->lineEdit()->setFocus();
            }
        } else if (((keyEvent->key() <= Qt::Key_Z && keyEvent->key() >= Qt::Key_A) ||
                    (keyEvent->key() <= Qt::Key_9 && keyEvent->key() >= Qt::Key_0) ||
                    (keyEvent->key() == Qt::Key_Space)) &&
                   keyEvent->modifiers() == Qt::NoModifier) {
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
                title_bar_proxy_->backwardButtonClicked();
                break;
            }
            case Qt::ForwardButton: {
                qDebug() << "eventFilter forward";
                title_bar_proxy_->forwardButtonClicked();
                break;
            }
            case Qt::MiddleButton: {
                return true;
            }
            default: {
            }
            }
        }
        if (!search_edit_->geometry().contains(this->mapFromGlobal(QCursor::pos()))) {
            completion_window_->hide();
        }
    }
    if (event->type() == QEvent::FontChange && watched == this) {
        if (this->settings_proxy_) {
            qDebug() << "eventFilter QEvent::FontChange";
            auto fontInfo = this->fontInfo();
            Q_EMIT this->settings_proxy_->fontChangeRequested(fontInfo.family(),
                                                              fontInfo.pixelSize());
        }
    }
    return QObject::eventFilter(watched, event);

}

void WebWindow::slot_ButtonHide()
{
//    QTimer::singleShot(20, [ = ]() {
//        qDebug() << "slot_ButtonHide";
//        buttonBox->hide();
//    });
    buttonBox->hide();
}

void WebWindow::slot_ButtonShow()
{
    QTimer::singleShot(20, [ = ]() {
        qDebug() << "slot_ButtonShow";
        buttonBox->show();
    });
}

void WebWindow::slot_ThemeChanged()
{
    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    if (themeType == DGuiApplicationHelper::LightType)
        web_view_->page()->setBackgroundColor(QColor("#F8F8F8"));
    else if (themeType == DGuiApplicationHelper::DarkType) {
        web_view_->page()->setBackgroundColor(QColor("#282828"));
    }
}

void WebWindow::slot_HelpSupportTriggered()
{
    qDebug() << "helpSupportTriggered";
}
}  // namespace dman
