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
#include <QWebEngineHistory>
#include <QTime>
#include <QClipboard>
#include <QNetworkProxyFactory>

#define SEARCH_EDIT_WIDTH 350  // 一般情况下搜索窗口的大小
#define SEARCH_EDIT_HEIGHT 44  // 一般情况下搜索窗口的大小
#define LIMIT_SEARCH_EDIT_WIDTH 750 // 当主窗口

namespace {

const int kSearchDelay = 200;

} // namespace

WebWindow::WebWindow(QWidget *parent)
    : Dtk::Widget::DMainWindow(parent)
    , search_timer_(new QTimer(this))
    , first_webpage_loaded_(true)
    , m_spinner(new DSpinner(this))
{
    // 使用 redirectContent 模式，用于内嵌 x11 窗口时能有正确的圆角效果
    Dtk::Widget::DPlatformWindowHandle::enableDXcbForWindow(this, true);
    search_timer_.setSingleShot(true);
    setAttribute(Qt::WA_InputMethodEnabled, true);

    //初始化窗口
    this->initUI();
    //初始化信号
    this->initConnections();
    //初始化快捷键
    this->initShortcuts();
    //初始化dbus服务
    this->initDBus();
    qApp->installEventFilter(this);
}

WebWindow::~WebWindow()
{
    //释放各个资源
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
    setSearchManager(); //防止启动后立即搜索响应测试不通过
    initWebView();
}

void WebWindow::updatePage(const QStringList &list)
{
    if (search_proxy_) {
        QStringList appList;
        for (const QString &app : list) {
            QStringList splitList = app.split("/");
            appList.append(splitList.at(splitList.count() - 4));
        }

        //发送到前端页面更新内容
        emit search_proxy_->reloadPage(appList);
    }

    if (search_manager_) {
        //重新获取应用列表
        //SearchDb::updateModule-->SearchDb::getAllApp
        emit search_manager_->updateModule();
    }
}

/**
 * @brief WebWindow::updateBtnBox 更新当前按钮组状态
 */
void WebWindow::updateBtnBox()
{
    if (m_forwardButton->isEnabled() || m_backButton->isEnabled()) {
        buttonBox->show();
        //wayland下， 窗口焦点在qwebengines上时，如果titlebar原有没有buttonbox，将titlebar上buttonbox->show（）后会出现黑方角，像是没有渲染完成一样。。。。
        //避免方法：刷新一次主窗口。。
        this->update();
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

/**
 * @brief WebWindow::openjsPage
 * @note 根据应用名和标题名定位到当前页面
 * @param app_name
 * @param title_name
 */
void WebWindow::openjsPage(const QString &app_name, const QString &title_name)
{
    if (bFinishChannel) {
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
                web_view_->page()->runJavaScript(QString("openTitle('%1','%2')").arg(app_name, title_name));
            }
        }
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
    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    QColor fillColor = DGuiApplicationHelper::instance()->applicationPalette().highlight().color();
    if(!Utils::judgeWayLand()){

        if (themeType == DGuiApplicationHelper::DarkType) {
            web_view_->page()->setBackgroundColor(QColor(37, 37, 37));
        } else {
            web_view_->page()->setBackgroundColor(QColor(248, 248, 248));
        }
    }else {
        if (themeType == DGuiApplicationHelper::DarkType) {
            web_view_->page()->setBackgroundColor(QColor(0x28, 0x28, 0x28));
            QPalette pa = palette();
            pa.setColor(QPalette::Window, QColor("#161616"));
            setPalette(pa);
        } else if (themeType == DGuiApplicationHelper::LightType) {
            QPalette pa = palette();
            pa.setColor(QPalette::Window, Qt::white);
            setPalette(pa);
            web_view_->page()->setBackgroundColor(QColor(248, 248, 248));
        }
    }
    web_view_->page()->runJavaScript(QString("setHashWordColor('%1')").arg(fillColor.name()));
}

/**
 * @brief WebWindow::HelpSupportTriggered  服务与支持响应处理
 * @param bActiontrigger   是否为菜单触发
 */
void WebWindow::HelpSupportTriggered(bool bActiontrigger)
{
    QDBusInterface interface("com.deepin.dde.ServiceAndSupport",
                             "/com/deepin/dde/ServiceAndSupport",
                             "com.deepin.dde.ServiceAndSupport");

    //    selfSupport = 0, //自助支持
    //    messageConsultation = 1, //留言咨询
    //    customerChat = 2, //在线客服
    //    contentUs = 3 //联系我们
    uint8_t supporttype = 0;
    if (!bActiontrigger) {
        supporttype = 2;
    }
    QDBusReply<void> reply = interface.call("ServiceSession", supporttype);
    if (reply.isValid()) {
        qDebug() << "call com.deepin.dde.ServiceAndSupport success";
    } else {
        qDebug() << "call com.deepin.dde.ServiceAndSupport failed";
    }
}

void WebWindow::slotUpdateLabel()
{
    sendMessage(QIcon(":/common/images/ok.svg"), QObject::tr("The content was updated"));
}

/**
 * @brief WebWindow::closeEvent
 * @param event
 * 重写关闭事件，保存窗口大小
 */
void WebWindow::closeEvent(QCloseEvent *event)
{
    //保存窗口大小
    saveWindowSize();
    QWidget::closeEvent(event);
}

/**
 * @brief WebWindow::resizeEvent
 * @param event
 * 拖拽调整窗口大小时重新设置搜索结果位置
 */
void WebWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    // 当窗口缩小时，搜索控件会被压缩，现调整为动态变化
    int detal = LIMIT_SEARCH_EDIT_WIDTH - event->size().width();
    if(detal <= 0){
        search_edit_->setFixedWidth(SEARCH_EDIT_WIDTH);
    }else if(detal != LIMIT_SEARCH_EDIT_WIDTH){
        search_edit_->setFixedWidth(SEARCH_EDIT_WIDTH - detal);
    }

    if(completion_window_->isVisible()){
        completion_window_->autoResize();
        // Move to below of search edit.
        const QPoint local_point(this->rect().width() / 2 - search_edit_->width() / 2,
                                 titlebar()->height() - 3);
        if(!Utils::judgeWayLand()){
            const QPoint global_point(this->mapToGlobal(local_point));
            completion_window_->move(global_point);
        }else {
            completion_window_->move(local_point);
        }
    }

    if(web_view_){
        slot_ThemeChanged();
    }
}

/**
 * @brief WebWindow::inputMethodEvent
 * @param e
 * 中文输入法支持
 */
void WebWindow::inputMethodEvent(QInputMethodEvent *e)
{
    if (!e->commitString().isEmpty() && search_edit_->lineEdit()->text() != e->commitString()) {
        search_edit_->lineEdit()->setText(e->commitString());
        search_edit_->lineEdit()->setFocus();
    }
    QWidget::inputMethodEvent(e);
}

/**
 * @brief WebWindow::inputMethodQuery
 * @param prop
 * @return
 * 中文输入法支持
 */
QVariant WebWindow::inputMethodQuery(Qt::InputMethodQuery prop) const
{
    switch (prop) {
    case Qt::ImEnabled:
        return true;
    default:
        break;
    }

    return QWidget::inputMethodQuery(prop);
}


/**
 * @brief WebWindow::eventFilter
 * @param watched 事件对象
 * @param event
 * @return
 * 事件过滤器
 */
bool WebWindow::eventFilter(QObject *watched, QEvent *event)
{
    //warland环境下watched的objectname不是QMainWindowClassWindow,先去除验证
    DIconButton* btn = findChild<DIconButton*>("DTitlebarDWindowMaxButton");
    if (event->type() == QEvent::MouseButtonRelease && qApp->activeWindow() == this && !btn->isDown()) {
        QRect rect = hasWidgetRect(search_edit_);
        if (web_view_ && web_view_->selectedText().isEmpty() && !rect.contains(QCursor::pos())) {
            this->setFocus();
        }
    }
    if (event->type() == QEvent::KeyPress && qApp->activeWindow() == this ) {
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
            //搜索框全选
            if (!search_edit_->lineEdit()->hasFocus()) {
                web_view_->setFocus(Qt::ActiveWindowFocusReason);
            }
        }
    }

    // Filters mouse press event only.
    if (event->type() == QEvent::MouseButtonPress && qApp->activeWindow() == this) {

        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if(nullptr != mouseEvent){
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
            case Qt::MiddleButton:
                return true;
            default:
                break;
            }
        }

        //wayland 当搜索结果隐藏之后,将无法触发鼠标点击事件无法跳转内容
        if(!Utils::judgeWayLand()){
            if (!hasWidgetRect(search_edit_).contains(mapFromGlobal(QCursor::pos()))) {
                completion_window_->hide();
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
 * @brief WebWindow::setSearchManager
 * 创建搜索管理类对象，绑定信号槽
 */
void WebWindow::setSearchManager()
{
    search_proxy_ = new SearchProxy(this);
    search_manager_ = new SearchManager(this);
    //searchContentResult绑定到JS页面
    connect(search_manager_, &SearchManager::searchContentResult, search_proxy_,
            &SearchProxy::onContentResult);
    //searchContentMismatch绑定到JS页面
    connect(search_manager_, &SearchManager::searchContentMismatch, search_proxy_,
            &SearchProxy::mismatch);
    connect(search_manager_, &SearchManager::searchAnchorResult, this,
            &WebWindow::onSearchAnchorResult);
}

/**
 * @brief WebWindow::initConnections
 * 绑定信号槽
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
}

/**
 * @brief WebWindow::onManualSearchByKeyword
 * @param keyword
 * 关键字搜索
 */
void WebWindow::onManualSearchByKeyword(const QString &keyword)
{
    qDebug() << "WebWindow: onManualSearchByKeyword keyword:" << keyword;
    this->onSearchContentByKeyword(keyword);
}

/**
 * @brief WebWindow::onAppearanceChanged
 * @param map 个性化对, QString:更改类型  QVariant:更改后的值
 * @note 改变系统个性化时，触发此槽
 */
void WebWindow::onAppearanceChanged(QString, QMap<QString, QVariant> map, QStringList)
{
    //20210630codereview
    if (map.isEmpty()) {
        return;
    }

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
    } else if (0 == strKey.compare("IconTheme") && nullptr != manual_proxy_) {
        //系统图表主题变更
        emit manual_proxy_->iconThemeChanged(strValue);
    }
}

/**
 * @brief WebWindow::initUI
 * @note 初始化窗口
 */
void WebWindow::initUI()
{
    //搜索结果框可移至主窗口创建完成后
    if(!Utils::judgeWayLand()){
        completion_window_ = new SearchCompletionWindow();
    }else {
        completion_window_ = new SearchCompletionWindow(this);
        completion_window_->hide();
    }

    setFocus(Qt::ActiveWindowFocusReason);
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
    search_edit_->setFixedSize(SEARCH_EDIT_WIDTH, SEARCH_EDIT_HEIGHT);
    search_edit_->setPlaceHolder(QObject::tr("Search"));
    if (Utils::hasSelperSupport()) {
        DMenu *pMenu = new DMenu;
        QAction *pHelpSupport = new QAction(QObject::tr("Support"));
        pMenu->addAction(pHelpSupport);
        this->titlebar()->setMenu(pMenu);
        connect(pHelpSupport, &QAction::triggered, this, [this] { this->HelpSupportTriggered(true); });
    }
    this->titlebar()->addWidget(buttonFrame, Qt::AlignLeft);
    this->titlebar()->addWidget(search_edit_, Qt::AlignCenter);
    this->titlebar()->setSeparatorVisible(false);
    this->titlebar()->setIcon(QIcon::fromTheme("deepin-manual"));

    if(Utils::judgeWayLand()){
        this->titlebar()->setAutoFillBackground(false);
        this->titlebar()->setBackgroundRole(QPalette::Window);
    }
    //隐藏title阴影
    this->setTitlebarShadowEnabled(false);
    //键盘盲打
    // search_edit_->setFocus();
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
    //图片控件
    image_viewer_ = new ImageViewer(this);
    //图片显示
    image_viewer_proxy_ = new ImageViewerProxy(image_viewer_, this);
    //主题代理类
    theme_proxy_ = new ThemeProxy(this);
    //设置代理类
    settings_proxy_ = new SettingsProxy(this);
    //语言代理类
    i18n_proxy = new I18nProxy(this);
    //手册处理代理类
    manual_proxy_ = new ManualProxy(this);
    //标题栏代理类
    title_bar_proxy_ = new TitleBarProxy(this);

    //上一页按钮
    connect(m_backButton, &DButtonBoxButton::clicked, title_bar_proxy_,
            &TitleBarProxy::backwardButtonClicked);
    //下一页按钮
    connect(m_forwardButton, &DButtonBoxButton::clicked, title_bar_proxy_,
            &TitleBarProxy::forwardButtonClicked);
    web_view_ = new QWebEngineView;
    web_view_->setAttribute(Qt::WA_KeyCompression, true);
    web_view_->setAttribute(Qt::WA_InputMethodEnabled, true);

    if(!Utils::judgeWayLand()){
        web_view_->setAttribute(Qt::WA_NativeWindow, true);
    }
    QNetworkProxyFactory::setUseSystemConfiguration(false);

    //禁止拖文件
    web_view_->setAcceptDrops(false);
    //使用该方法效果最好但使用后消息提示控件不可见,所以根据主题设置相适应的背景色
    // web_view_->page()->setBackgroundColor(Qt::transparent);
    //根据系统主题设置web背景色
    slot_ThemeChanged();
    QWebChannel *web_channel = new QWebChannel;
    //向web页面注册C++类对象
    web_channel->registerObject("i18n", i18n_proxy);
    web_channel->registerObject("imageViewer", image_viewer_proxy_);
    web_channel->registerObject("manual", manual_proxy_);
    web_channel->registerObject("search", search_proxy_);
    web_channel->registerObject("theme", theme_proxy_);
    web_channel->registerObject("titleBar", title_bar_proxy_);
    web_channel->registerObject("settings", settings_proxy_);
    web_view_->page()->setWebChannel(web_channel);

    connect(web_view_->page(), &QWebEnginePage::loadProgress, this, &WebWindow::onWebPageLoadProgress);
    connect(manual_proxy_, &ManualProxy::channelInit, this, &WebWindow::onChannelFinish);
    connect(manual_proxy_, &ManualProxy::WidgetLower, this, &WebWindow::lower);
    connect(manual_proxy_, &ManualProxy::updateLabel, this, &WebWindow::slotUpdateLabel);
    connect(manual_proxy_, &ManualProxy::supportBeClick, this, [this] { this->HelpSupportTriggered(); });

    //web主页html路径
    const QFileInfo info(kIndexPage);
    web_view_->load(QUrl::fromLocalFile(info.absoluteFilePath()));
    connect(search_edit_, &SearchEdit::onClickedClearBtn, manual_proxy_,
            &ManualProxy::searchEditTextisEmpty);
    connect(search_proxy_, &SearchProxy::setKeyword, this, &WebWindow::onSetKeyword);
    connect(search_proxy_, &SearchProxy::updateSearchResult, this, &WebWindow::onTitleBarEntered);

    //接收系统主题变化信号发送给前端JS
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            theme_proxy_, &ThemeProxy::slot_ThemeChange);

    //接收系统主题变化信号
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            this, &WebWindow::slot_ThemeChanged);

    manual_proxy_->setApplicationState("dde");
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
        if (this->windowState() & Qt::WindowMaximized){
            this->showNormal();
        } else if (this->windowState() == Qt::WindowNoState){
            this->showMaximized();
        }
    });

    //设置搜索快捷键  后期将支持盲打功能,故不需要此快捷键
    QShortcut *scSearch = new QShortcut(this);
    scSearch->setKey(tr("Ctrl+F"));
    scSearch->setContext(Qt::WindowShortcut);
    scSearch->setAutoRepeat(false);
    connect(scSearch, &QShortcut::activated, this, [this] {
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
                "com.deepin.daemon.Appearance", // sender's service name
                "/com/deepin/daemon/Appearance", // sender's path name
                "org.freedesktop.DBus.Properties", // interface
                "PropertiesChanged", // sender's signal name
                this, // receiver
                SLOT(onAppearanceChanged(QString, QMap<QString, QVariant>, QStringList)))) {
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
    //文案内容右键显示复制
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
 * @brief WebWindow::hasWidgetRect
 * @return 返回控件Rect位置
 * 根据父控件位置确定子控件位置
 */
QRect WebWindow::hasWidgetRect(QWidget *widget)
{
    QRect rect;
    QWidget *tmpWidget = widget;
    while (tmpWidget) {
        rect.setX(rect.x() + tmpWidget->x());
        rect.setY(rect.y() + tmpWidget->y());
        tmpWidget = tmpWidget->parentWidget();
    }
    rect.setWidth(widget->width());
    rect.setHeight(widget->height());
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
    const QString searchKey = key.remove('\n').remove('\r').remove("\r\n").remove(QRegExp("\\s"));
    //在数据库中查询->SearchDb::searchContent->SearchDb::handleSearchContent
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
    //根据关键字在内容中查找
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

/**
 * @brief WebWindow::onSearchTextChangedDelay
 * 获取searchEidt中的文本，过滤特殊字符与中隔，设置searchButton显示文本
 */
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

/**
 * @brief WebWindow::onTitleBarEntered
 * 焦点在SearchEdit中，按回车时调用 ，调用onEnterPressed()进行搜索操作
 */
void WebWindow::onTitleBarEntered()
{
    qDebug() << Q_FUNC_INFO;
    QString textTemp = search_edit_->text();
    const QString text = textTemp.remove('\n').remove('\r').remove("\r\n").remove(QRegExp("\\s"));
    if (text.size() >= 1) {
        completion_window_->onEnterPressed();
    }
}

/**
 * @brief WebWindow::onWebPageLoadProcess qwebengines加载进度响应槽
 * @valpro  网页加载进度响应
 * 网页加载达到20%就显示提什用户体验
 */
void WebWindow::onWebPageLoadProgress(int valpro)
{
    if (m_spinner->isPlaying() && valpro > 20) {
        m_spinner->stop();
        m_spinner->hide();
        this->setCentralWidget(web_view_);
        disconnect(web_view_->page(), &QWebEnginePage::loadProgress, this, &WebWindow::onWebPageLoadProgress);
    }
}

/**
 * @brief WebWindow::onChannelFinish
 * 完成channel对象与Qt对象绑定后回调
 */
void WebWindow::onChannelFinish()
{
    bFinishChannel = true;
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
            //清空搜索框
            search_edit_->clearEdit();
        } else {
            QString strTemp = keyword;
            if (strTemp.contains("=-=")) {
                strTemp.replace("=-=", "%");
            }
            //设置搜索框值
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
        if(!Utils::judgeWayLand()){
            const QPoint global_point(this->mapToGlobal(local_point));
            completion_window_->move(global_point);
        }else {
            completion_window_->move(local_point);
        }

        completion_window_->setFocusPolicy(Qt::NoFocus);
        completion_window_->setFocusPolicy(Qt::StrongFocus);
    }
}
