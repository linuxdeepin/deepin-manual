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
#include "base/utils.h"
#include "controller/search_manager.h"
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

#include <DPlatformWindowHandle>
#include <DTitlebar>

#include <QApplication>
#include <QShortcut>
#include <QShowEvent>
#include <QWebChannel>
#include <QWindow>
#include <QX11Info>

#include <QRegion>
#include <QFocusEvent>

#include "base/utils.h"

DWIDGET_USE_NAMESPACE

namespace dman {

namespace {

const int kSearchDelay = 200;

} // namespace

WebWindow::WebWindow(QWidget *parent)
    : Dtk::Widget::DMainWindow(parent)
    , search_timer_(new QTimer)
    , first_webpage_loaded_(true)
    , m_spinner(new DSpinner)
{
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    // 使用 redirectContent 模式，用于内嵌 x11 窗口时能有正确的圆角效果
    Dtk::Widget::DPlatformWindowHandle::enableDXcbForWindow(this, true);
    search_timer_.setSingleShot(true);
    setAttribute(Qt::WA_InputMethodEnabled, true);

    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    this->initUI();
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    this->initConnections();
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    this->initShortcuts();
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    this->initDBus();
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");

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
    if (search_manager_ != nullptr) {
        search_manager_->deleteLater();
        search_manager_ = nullptr;
    }
    if (image_viewer_ != nullptr) {
        image_viewer_->deleteLater();
        image_viewer_ = nullptr;
    }
    if (i18n_proxy != nullptr) {
        i18n_proxy->deleteLater();
        i18n_proxy = nullptr;
    }
    if (buttonBox != nullptr) {
        buttonBox->deleteLater();
        buttonBox = nullptr;
    }
    if (search_edit_ != nullptr) {
        search_edit_->deleteLater();
        search_edit_ = nullptr;
    }
}

/**
 * @brief WebWindow::initWeb 初始化web页面. 当主窗口show()完成后执行, 减少启动时间..
 */
void WebWindow::initWeb()
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    initWebView();
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    setSearchManager();
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    const QFileInfo info(kIndexPage);
    web_view_->load(QUrl::fromLocalFile(info.absoluteFilePath()));
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
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
 * @brief WebWindow::cancelTextChanged
 * @note 取消文本选中状态，用于在帮助手册首页时不显示＂Ｃopy＂按钮
 * 每次切换页面时取消显示menu，当selectionChanged触发时设置显示menu
 */
void WebWindow::cancelTextChanged()
{
    web_view_->setContextMenuPolicy(Qt::NoContextMenu);
}

void WebWindow::openjsPage(const QString &app_name, const QString &title_name)
{
    if (app_name.isEmpty()) {
        qDebug() << "===========>";
        qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
        web_view_->page()->runJavaScript("index()");
        qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
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

/**
 * @brief WebWindow::setAppProperty 设置参数属性
 * @param appName   模块名称
 * @param titleName 标题名称
 * @param keyword   搜索关键字
 */
void WebWindow::setAppProperty(const QString &appName, const QString &titleName, const QString &keyword)
{
    keyword_ = keyword;
    app_name_ = appName;
    title_name_ = titleName;
}

/**
 * @brief WebWindow::slot_ThemeChanged
 * @note 修改背景颜色，　减少WebEngeineView 控件在黑色主题下的明显闪屏效果
 */
void WebWindow::slot_ThemeChanged()
{
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    if (themeType == DGuiApplicationHelper::LightType) {
        qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
        web_view_->page()->setBackgroundColor(QColor(0xF8, 0xF8, 0xF8));
        qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    } else if (themeType == DGuiApplicationHelper::DarkType) {
        web_view_->page()->setBackgroundColor(QColor(0x28, 0x28, 0x28));
    }
}

/**
 * @brief WebWindow::slot_HelpSupportTriggered  服务与支持触发槽
 */
void WebWindow::slot_HelpSupportTriggered()
{
    QDBusInterface interface("com.deepin.dde.ServiceAndSupport",
                                 "/com/deepin/dde/ServiceAndSupport",
                                 "com.deepin.dde.ServiceAndSupport");

    QDBusReply<void> reply = interface.call("ShowCustomerChat");
    if (reply.isValid()) {
        qDebug() << "call com.deepin.dde.ServiceAndSupport success";
    } else {
        qDebug() << "call com.deepin.dde.ServiceAndSupport failed";
    }
}

void WebWindow::closeEvent(QCloseEvent *event)
{
    //保存窗口大小
    saveWindowSize();
    QWidget::closeEvent(event);
}

void WebWindow::inputMethodEvent(QInputMethodEvent *e)
{
    if (!e->commitString().isEmpty()) {
        search_edit_->lineEdit()->setText(e->commitString());
        search_edit_->lineEdit()->setFocus();
    }

    QWidget::inputMethodEvent(e);
}

bool WebWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease && qApp->activeWindow() == this &&
            watched->objectName() == QLatin1String("QMainWindowClassWindow")) {
        QRect rect = hasSearchEditRect();
        if (web_view_ && web_view_->selectedText().isEmpty() && !rect.contains(QCursor::pos())) {
            this->setFocus();
        }
    }
    if (event->type() == QEvent::KeyPress && qApp->activeWindow() == this &&
            watched->objectName() == QLatin1String("QMainWindowClassWindow")) {
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
        } else if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
            if (search_edit_->lineEdit()->hasFocus()) {
                //搜索框内容为空时，按回车键回到未搜索页面
                if (search_edit_->lineEdit()->text().isEmpty()) {
                    emit manual_proxy_->searchEditTextisEmpty();
                    qDebug() << "emit searchEditTextisEmpty";
                }
            }
        } else if (keyEvent->key() == Qt::Key_A
                   && keyEvent->modifiers().testFlag(Qt::ControlModifier)) {
            web_view_->setFocus(Qt::ActiveWindowFocusReason);
        }
    }

    // Filters mouse press event only.
    if (event->type() == QEvent::MouseButtonPress && qApp->activeWindow() == this &&
            watched->objectName() == QLatin1String("QMainWindowClassWindow")) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        //        this->web_view_->update();
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

void WebWindow::setSearchManager()
{
    search_manager_ = new SearchManager(this);
    connect(search_manager_, &SearchManager::searchContentResult, search_proxy_,
            &SearchProxy::onContentResult);
    connect(search_manager_, &SearchManager::searchContentMismatch, search_proxy_,
            &SearchProxy::mismatch);
    connect(search_manager_, &SearchManager::searchAnchorResult, this,
            &WebWindow::onSearchAnchorResult);
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
    }
}

/**
 * @brief WebWindow::initUI
 * @note 初始化窗口
 */
void WebWindow::initUI()
{
    //搜索结果框可移至主窗口创建完成后
    completion_window_ = new SearchCompletionWindow();

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

    search_edit_ = new SearchEdit;
    DFontSizeManager::instance()->bind(search_edit_, DFontSizeManager::T6, QFont::Normal);
    search_edit_->setObjectName("SearchEdit");
    search_edit_->setFixedSize(350, 44);
    search_edit_->setPlaceHolder(QObject::tr("Search"));
    search_edit_->setContextMenuPolicy(Qt::CustomContextMenu);

    if (Utils::hasSelperSupport()) {
        DMenu *pMenu = new DMenu;
        QAction *pHelpSupport = new QAction(QObject::tr("Support"));
        pMenu->addAction(pHelpSupport);
        this->titlebar()->setMenu(pMenu);
        connect(pHelpSupport, &QAction::triggered, this, &WebWindow::slot_HelpSupportTriggered);
    }
    this->titlebar()->addWidget(buttonFrame, Qt::AlignLeft);
    this->titlebar()->addWidget(search_edit_, Qt::AlignCenter);
    this->titlebar()->setSeparatorVisible(false);
    this->titlebar()->setIcon(QIcon::fromTheme("deepin-manual"));
    //隐藏title阴影
    this->setTitlebarShadowEnabled(false);
    //键盘盲打
    search_edit_->setFocus();
    this->setFocusPolicy(Qt::ClickFocus);


    QWidget *spinnerPage = new QWidget;
    QVBoxLayout *spinnerLayout = new QVBoxLayout(spinnerPage);
    m_spinner->setFixedSize(50, 50);
    spinnerLayout->addWidget(m_spinner, 0, Qt::AlignCenter);
    this->setCentralWidget(spinnerPage);
    m_spinner->start();
}

/**
 * @brief WebWindow::initWebView
 * @note 初始化qwebengines, 初始化QWebChannel,通过QWebChannel完成QT与JS间通讯
 */
void WebWindow::initWebView()
{
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    image_viewer_ = new ImageViewer(this);
    image_viewer_proxy_ = new ImageViewerProxy(image_viewer_, this);
    theme_proxy_ = new ThemeProxy(this);
    settings_proxy_ = new SettingsProxy(this);
    i18n_proxy = new I18nProxy(this);
    manual_proxy_ = new ManualProxy(this);
    search_proxy_ = new SearchProxy(this);
    title_bar_proxy_ = new TitleBarProxy(this);
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");

    connect(m_backButton, &DButtonBoxButton::clicked, title_bar_proxy_,
            &TitleBarProxy::backwardButtonClicked);
    connect(m_forwardButton, &DButtonBoxButton::clicked, title_bar_proxy_,
            &TitleBarProxy::forwardButtonClicked);
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    web_view_ = new QWebEngineView;
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
//    this->setCentralWidget(web_view_);
//    web_view_->hide();
    web_view_->setAcceptDrops(false);
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    slot_ThemeChanged();
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    QWebChannel *web_channel = new QWebChannel;
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    web_channel->registerObject("i18n", i18n_proxy);
    web_channel->registerObject("imageViewer", image_viewer_proxy_);
    web_channel->registerObject("manual", manual_proxy_);
    web_channel->registerObject("search", search_proxy_);
    web_channel->registerObject("theme", theme_proxy_);
    web_channel->registerObject("titleBar", title_bar_proxy_);
    web_channel->registerObject("settings", settings_proxy_);
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    web_view_->page()->setWebChannel(web_channel);
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");

    connect(web_view_->page(), &QWebEnginePage::loadFinished, this, &WebWindow::onWebPageLoadFinished);
    connect(manual_proxy_, &ManualProxy::channelInit, this, &WebWindow::onChannelFinish);
    connect(manual_proxy_, &ManualProxy::WidgetLower, this, &WebWindow::lower);
    connect(manual_proxy_, &ManualProxy::supportBeClick, this, &WebWindow::slot_HelpSupportTriggered);
    connect(search_edit_, &SearchEdit::onClickedClearBtn, manual_proxy_,
            &ManualProxy::searchEditTextisEmpty);
    connect(search_proxy_, &SearchProxy::setKeyword, this, &WebWindow::onSetKeyword);
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            theme_proxy_, &ThemeProxy::slot_ThemeChange);
//    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
//            this, &WebWindow::slot_ThemeChanged);

    manual_proxy_->setApplicationState("dde");
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
}

/**
 * @brief WebWindow::saveWindowSize 记录最后一个正常窗口的大小
 */
void WebWindow::saveWindowSize()
{
    QSettings *setting = ConfigManager::getInstance()->getSettings();
    setting->beginGroup(kConfigWindowInfo);
    setting->setValue(kConfigWindowWidth, width());
    setting->setValue(kConfigWindowHeight, height());
    setting->endGroup();
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

//    //设置全选切换快捷键
//    QShortcut *sCheckAll = new QShortcut(this);
//    sCheckAll->setKey(tr("Ctrl+A"));
//    sCheckAll->setContext(Qt::WindowShortcut);
//    sCheckAll->setAutoRepeat(false);
//    connect(sCheckAll, &QShortcut::activated, this, [this] {

//        web_view_->setFocus();
//    });

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
                "com.deepin.daemon.Appearance",     // sender's service name
                "/com/deepin/daemon/Appearance",    // sender's path name
                "org.freedesktop.DBus.Properties",  // interface
                "PropertiesChanged",                // sender's signal name
                this,                               // receiver
                SLOT(onACtiveColorChanged(QString, QMap<QString, QVariant>, QStringList)))) {
        qDebug() << "connectToBus()::connect()  PropertiesChanged failed";
    } else {
        qDebug() << "connectToBus()::connect()  PropertiesChanged success";
    }
}

/**
 * @brief WebWindow::setHashWordColor
 * 调用ｊｓ接口，改变活动色
 */
void WebWindow::setHashWordColor()
{
    QColor Color = DGuiApplicationHelper::instance()->applicationPalette().highlight().color();
    QString strColor = Color.name(QColor::NameFormat::HexRgb);
    web_view_->page()->runJavaScript(QString("setHashWordColor('%1')").arg(strColor));
    completion_window_->updateColor(Color);
}

/**
 * @brief WebWindow::settingContextMenu
 * 初始化鼠标选中内容右键菜单Menu.
 */
void WebWindow::settingContextMenu()
{
    web_view_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(web_view_, &QWebEngineView::selectionChanged, this, [this]() {
        web_view_->setContextMenuPolicy(Qt::CustomContextMenu);
    });
    QMenu *menu = new QMenu(this);
    QAction *action = menu->addAction(QObject::tr("Copy"));
    connect(web_view_, &QWidget::customContextMenuRequested, this, [ = ]() {
        if (!web_view_->selectedText().isEmpty()) {
            menu->exec(QCursor::pos());
        }
    });
    connect(action, &QAction::triggered, this, [ = ]() {
        QApplication::clipboard()->setText(web_view_->selectedText());
    });
}

/**
 * @brief WebWindow::hasSearchEditRect
 * @return 返回SearchEdit的绝对坐标范围
 */
QRect WebWindow::hasSearchEditRect()
{
    QRect rect;
    QList<QWidget *> list = this->titlebar()->findChildren<QWidget *>();
    for (int i = 0 ; i < list.length(); ++i) {
        if (list.at(i)->inherits("QLabel")) {
            if (list.at(i)->width() == 28 || list.at(i)->width() == 640) {
                continue;
            }
            int x = (list.at(i)->width() - search_edit_->width()) / 2;
            int y = (list.at(i)->height() - search_edit_->height()) / 2;
            QPoint gp = list.at(i)->mapToGlobal(list.at(i)->pos());
            QPoint p(gp.x() + x - 200, gp.y() + y);
            //QRect rect(p1.x(), p1.y(), search_edit_->width(), search_edit_->height());
            rect.setX(p.x());
            rect.setY(p.y());
            rect.setWidth(search_edit_->width());
            rect.setHeight(search_edit_->height());
            return rect;
        }
    }
    return rect;
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
    search_manager_->searchContent(searchKey);

    QString base64Key = QString(searchKey.toUtf8().toBase64());
    qDebug() << " base64Key " << base64Key << endl;

    // 调用ｊｓ接口显示搜索内容
    web_view_->page()->runJavaScript(QString("openSearchPage('%1')").arg(base64Key));
}

/**
 * @brief WebWindow::onSearchEditFocusOut
 * 搜索框失去焦点
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
    QString textTemp = search_edit_->text();
    const QString text = textTemp.remove('\n').remove('\r').remove("\r\n");
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
    web_view_->page()->runJavaScript(QString("open('%1', '%2', '%3')").arg(result.app_name, result.anchorId, result.anchor));
}

/**
 * @brief WebWindow::onSearchTextChanged
 * @param text
 * @note ２００ｍｓ内搜索框中的文本改变时重新计时执行一次搜索操作, 200ms搜索一次
 */
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
    // 过滤特殊字符
    if (text.size() < 1 || text.toLower().contains(QRegExp("[+-_$!@#%^&\\(\\)]"))) {
        return;
    }
    completion_window_->setKeyword(text);
    //执行搜索
    if (search_manager_) {
        emit search_manager_->searchAnchor(text);
    }
}

void WebWindow::onTitleBarEntered()
{
    QString textTemp = search_edit_->text();
    const QString text = textTemp.remove('\n').remove('\r').remove("\r\n").remove(QRegExp("\\s"));
    if (text.size() >= 1) {
        completion_window_->onEnterPressed();
    }
    //    else if (textTemp.isEmpty()) {
    //        m_backButton->click();
    //    }
}

/**
 * @brief WebWindow::onWebPageLoadFinished qwebengines加载页面完成槽
 * @note  网页加载完成后设置其配置
 * @param ok
 */
void WebWindow::onWebPageLoadFinished(bool ok)
{
    Q_UNUSED(ok)
    //settingContextMenu();
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
 * @brief WebWindow::onChannelFinish
 * 完成channel对象与Qt对象绑定后回调
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


    m_spinner->stop();
    m_spinner->hide();
    this->setCentralWidget(web_view_);
    web_view_->show();
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
}

/**
 * @brief WebWindow::onSetKeyword JS根据页面关键字回调设置搜索框
 * @param keyword 关键字
 * @note keyword 可能存在转换后的字符,所以需要将字符反向转换回去.
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
            QString strTemp = keyword;
            if (strTemp.contains("-+")) {
                strTemp.replace("-+", "/");
            }
            search_edit_->setText(strTemp);
        }
    }
}

/**
 * @brief WebWindow::onSearchAnchorResult 搜索结果框内容显示
 * @param keyword 搜索关键字
 * @param result  搜索的内容
 */
void WebWindow::onSearchAnchorResult(const QString &keyword, const SearchAnchorResultList &result)
{
    //搜索结果不存在，忽略这个信号
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


}  // namespace dman
