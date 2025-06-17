// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "view/web_window.h"
#include "base/consts.h"
#include "base/utils.h"
#include "base/ddlog.h"
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
#include <DPalette>
#include <DSysInfo>

#include <QApplication>
#include <QShortcut>
#include <QShowEvent>
#include <QWebChannel>
#include <QWindow>
#include <QRegion>
#include <QFocusEvent>
#include <QWebEngineHistory>
#include <QTime>
#include <QClipboard>
#include <QNetworkProxyFactory>
#include <QScreen>

DCORE_USE_NAMESPACE

#define SEARCH_EDIT_WIDTH 350  // 一般情况下搜索窗口的大小
#define SEARCH_EDIT_HEIGHT 44  // 一般情况下搜索窗口的大小
#define LIMIT_SEARCH_EDIT_WIDTH 750 // 当主窗口

namespace {

const int kSearchDelay = 200;
const int BUTTON_SIZE  = 36;
const int BUTTON_SIZE_COMPACT = 24;
} // namespace

WebWindow::WebWindow(QWidget *parent)
    : Dtk::Widget::DMainWindow(parent)
    , search_timer_(new QTimer(this))
    , first_webpage_loaded_(true)
    , m_spinner(new DSpinner(this))
{
    qCDebug(app) << "WebWindow constructor called";

    // 使用 redirectContent 模式，用于内嵌 x11 窗口时能有正确的圆角效果
    Dtk::Widget::DPlatformWindowHandle::enableDXcbForWindow(this, true);
    search_timer_.setSingleShot(true);
    setAttribute(Qt::WA_InputMethodEnabled, true);

    qCDebug(app) << "Initializing web window UI";
    //初始化窗口
    this->initUI();
    //初始化信号
    this->initConnections();
    //初始化快捷键
    this->initShortcuts();
    //初始化dbus服务
    this->initDBus();
    qApp->installEventFilter(this);
    qCDebug(app) << "WebWindow constructor done.";
}

WebWindow::~WebWindow()
{
    qCDebug(app) << "WebWindow destructor called";

    //释放各个资源
    if (completion_window_ != nullptr) {
        delete completion_window_;
        completion_window_ = nullptr;
        qCDebug(app) << "completion_window_ released";
    }
    if (image_viewer_proxy_ != nullptr) {
        image_viewer_proxy_->deleteLater();
        image_viewer_proxy_ = nullptr;
        qCDebug(app) << "image_viewer_proxy_ released";
    }
    if (search_proxy_ != nullptr) {
        search_proxy_->deleteLater();
        search_proxy_ = nullptr;
        qCDebug(app) << "search_proxy_ released";
    }
    if (theme_proxy_ != nullptr) {
        theme_proxy_->deleteLater();
        theme_proxy_ = nullptr;
        qCDebug(app) << "theme_proxy_ released";
    }
    if (manual_proxy_ != nullptr) {
        manual_proxy_->deleteLater();
        manual_proxy_ = nullptr;
        qCDebug(app) << "manual_proxy_ released";
    }
    if (title_bar_proxy_ != nullptr) {
        title_bar_proxy_->deleteLater();
        title_bar_proxy_ = nullptr;
        qCDebug(app) << "title_bar_proxy_ released";
    }
    if (settings_proxy_ != nullptr) {
        settings_proxy_->deleteLater();
        settings_proxy_ = nullptr;
        qCDebug(app) << "settings_proxy_ released";
    }
    if (search_manager_ != nullptr) {
        search_manager_->deleteLater();
        search_manager_ = nullptr;
        qCDebug(app) << "search_manager_ released";
    }
    if (image_viewer_ != nullptr) {
        image_viewer_->deleteLater();
        image_viewer_ = nullptr;
        qCDebug(app) << "image_viewer_ released";
    }
    if (i18n_proxy != nullptr) {
        i18n_proxy->deleteLater();
        i18n_proxy = nullptr;
        qCDebug(app) << "i18n_proxy released";
    }
    if (buttonBox != nullptr) {
        buttonBox->deleteLater();
        buttonBox = nullptr;
        qCDebug(app) << "buttonBox released";
    }
    if (search_edit_ != nullptr) {
        search_edit_->deleteLater();
        search_edit_ = nullptr;
        qCDebug(app) << "search_edit_ released";
    }

    qCDebug(app) << "WebWindow::~WebWindow() done.";
}

/**
 * @brief WebWindow::initWeb 初始化web页面. 当主窗口show()完成后执行, 减少启动时间..
 */
void WebWindow::initWeb()
{
    qCDebug(app) << "WebWindow::initWeb()";

    setSearchManager(); //防止启动后立即搜索响应测试不通过
    initWebView();
    qCDebug(app) << "WebWindow::initWeb() done.";
}

void WebWindow::updatePage(const QStringList &list)
{
    qCDebug(app) << "Updating page with list size:" << list.size();

    if (search_proxy_) {
        qCDebug(app) << "Is search proxy";
        QStringList appList;
        for (const QString &app : list) {
            QStringList splitList = app.split("/");
            appList.append(splitList.at(splitList.count() - 4));
        }
        qCDebug(app) << "appList size:" << appList.size();

        //发送到前端页面更新内容
        emit search_proxy_->reloadPage(appList);
    }

    if (search_manager_) {
        qCDebug(app) << "Is search manager";
        //重新获取应用列表
        //SearchDb::updateModule-->SearchDb::getAllApp
        emit search_manager_->updateModule();
    }
    qCDebug(app) << "WebWindow::updatePage() done.";
}

/**
 * @brief WebWindow::updateBtnBox 更新当前按钮组状态
 */
void WebWindow::updateBtnBox()
{
    qCDebug(app) << "WebWindow::updateBtnBox()";
    if (m_forwardButton->isEnabled() || m_backButton->isEnabled()) {
        qCDebug(app) << "Forward or backward button enabled";
        buttonBox->show();
        //wayland下， 窗口焦点在qwebengines上时，如果titlebar原有没有buttonbox，将titlebar上buttonbox->show（）后会出现黑方角，像是没有渲染完成一样。。。。
        //避免方法：刷新一次主窗口。。
        this->update();
    } else {
        qCDebug(app) << "Forward or backward button disabled";
        buttonBox->hide();
    }
    qCDebug(app) << "WebWindow::updateBtnBox() done.";
}

/**
 * @brief WebWindow::cancelTextChanged
 * @note 取消文本选中状态，用于在帮助手册首页时不显示＂Ｃopy＂按钮
 * 每次切换页面时取消显示menu，当selectionChanged触发时设置显示menu
 */
void WebWindow::cancelTextChanged()
{
    qCDebug(app) << "WebWindow::cancelTextChanged()";
    web_view_->setContextMenuPolicy(Qt::NoContextMenu);
    qCDebug(app) << "WebWindow::cancelTextChanged() done.";
}

/**
 * @brief WebWindow::openjsPage
 * @note 根据应用名和标题名定位到当前页面
 * @param app_name
 * @param title_name
 */
void WebWindow::openjsPage(const QString &app_name, const QString &title_name)
{
    qCDebug(app) << "WebWindow::openjsPage()";
    if (bFinishChannel) {
        if (app_name.isEmpty()) {
            qCDebug(app) << "App name is empty, open index page";
            web_view_->page()->runJavaScript("index()");
        } else {
            QString real_path(app_name);
            if (real_path.contains('/')) {
                qCDebug(app) << "App name contains '/', treating as absolute path";
                // Open markdown file with absolute path.
                QFileInfo info(real_path);
                real_path = info.canonicalFilePath();
                web_view_->page()->runJavaScript(QString("open('%1')").arg(real_path));
            } else {
                qCDebug(app) << "App name is not absolute path, open system manual";
                // Open system manual.
                web_view_->page()->runJavaScript(QString("openTitle('%1','%2')").arg(app_name, title_name));
            }
        }
    }
    qCDebug(app) << "WebWindow::openjsPage() done.";
}

/**
 * @brief WebWindow::setAppProperty 设置参数属性
 * @param appName   模块名称
 * @param titleName 标题名称
 * @param keyword   搜索关键字
 */
void WebWindow::setAppProperty(const QString &appName, const QString &titleName, const QString &keyword)
{
    qCDebug(app) << "WebWindow::setAppProperty()";
    keyword_ = keyword;
    app_name_ = appName;
    title_name_ = titleName;
    qCDebug(app) << "WebWindow::setAppProperty() done.";
}

/**
 * @brief WebWindow::slot_ThemeChanged
 * @note 修改背景颜色，　减少WebEngeineView 控件在黑色主题下的明显闪屏效果
 */
void WebWindow::slot_ThemeChanged()
{
    qCDebug(app) << "Theme changed, current type:" << DGuiApplicationHelper::instance()->themeType();

    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    QColor fillColor = DGuiApplicationHelper::instance()->applicationPalette().highlight().color();
    if (!Utils::judgeWayLand()) {
        qCDebug(app) << "Not wayland, set background color";
        if (themeType == DGuiApplicationHelper::DarkType) {
            qCDebug(app) << "Dark theme, set background color";
            web_view_->page()->setBackgroundColor(QColor(37, 37, 37));
        } else {
            qCDebug(app) << "Light theme, set background color";
            web_view_->page()->setBackgroundColor(QColor(248, 248, 248));
        }
    } else {
        qCDebug(app) << "Wayland, set background color and palette";
        if (themeType == DGuiApplicationHelper::DarkType) {
            qCDebug(app) << "Dark theme, set background color and palette";
            web_view_->page()->setBackgroundColor(QColor(0x28, 0x28, 0x28));
            QPalette pa = palette();
            pa.setColor(QPalette::Window, QColor("#161616"));
            setPalette(pa);
        } else if (themeType == DGuiApplicationHelper::LightType) {
            qCDebug(app) << "Light theme, set background color and palette";
            QPalette pa = palette();
            pa.setColor(QPalette::Window, Qt::white);
            setPalette(pa);
            web_view_->page()->setBackgroundColor(QColor(248, 248, 248));
        }
    }
    qCDebug(app) << "WebWindow::slot_ThemeChanged() done.";
    web_view_->page()->runJavaScript(QString("setHashWordColor('%1')").arg(fillColor.name()));
}

/**
 * @brief WebWindow::HelpSupportTriggered  服务与支持响应处理
 * @param bActiontrigger   是否为菜单触发
 */
void WebWindow::HelpSupportTriggered(bool bActiontrigger)
{
    qCDebug(app) << "WebWindow::HelpSupportTriggered()";
    QDBusInterface interface("com.deepin.dde.ServiceAndSupport",
                                 "/com/deepin/dde/ServiceAndSupport",
                                 "com.deepin.dde.ServiceAndSupport");

    //    selfSupport = 0, //自助支持
    //    messageConsultation = 1, //留言咨询
    //    customerChat = 2, //在线客服
    //    contentUs = 3 //联系我们
    uint8_t supporttype = 0;
    if (!bActiontrigger) {
        qCDebug(app) << "Menu triggered";
        supporttype = 2;
    }
    QDBusReply<void> reply = interface.call("ServiceSession", supporttype);
    if (reply.isValid()) {
        qCDebug(app) << "call com.deepin.dde.ServiceAndSupport success";
    } else {
        qCWarning(app) << "call com.deepin.dde.ServiceAndSupport failed, " << interface.lastError();
        // 打开失败，尝试再次打开
        QTimer::singleShot(100, this, [bActiontrigger](){
            QDBusInterface interface("com.deepin.dde.ServiceAndSupport",
                                     "/com/deepin/dde/ServiceAndSupport",
                                     "com.deepin.dde.ServiceAndSupport");
            uint8_t supporttype = 0;
            if (!bActiontrigger) {
                qCDebug(app) << "Menu triggered, call again";
                supporttype = 2;
            }
            QDBusReply<void> reply = interface.call("ServiceSession", supporttype);
            if (!reply.isValid()) {
                qCWarning(app) << "call failed again";
            }
        });
    }
    qCDebug(app) << "WebWindow::HelpSupportTriggered() done.";
}

void WebWindow::appStoreTriggered()
{
    qCDebug(app) << "WebWindow::appStoreTriggered()";
    if (!Utils::hasAppStore()) {
        qCDebug(app) << "App store not installed";
        return;
    }

    QProcess process;
    process.startDetached("deepin-home-appstore-client");
    process.waitForFinished();
    qCDebug(app) << "WebWindow::appStoreTriggered() done.";
}

void WebWindow::slotUpdateLabel()
{
    qCDebug(app) << "WebWindow::slotUpdateLabel()";
    sendMessage(QIcon(":/common/images/ok.svg"), QObject::tr("The content was updated"));
    qCDebug(app) << "WebWindow::slotUpdateLabel() done.";
}

void WebWindow::updateSizeMode()
{
    qCDebug(app) << "WebWindow::updateSizeMode()";
    int nBtnSize = BUTTON_SIZE;
#ifdef DTKWIDGET_CLASS_DSizeMode
    if (DGuiApplicationHelper::isCompactMode())
        nBtnSize = BUTTON_SIZE_COMPACT;
    else
        nBtnSize = BUTTON_SIZE;
#else
    nBtnSize = BUTTON_SIZE;
#endif
    qCDebug(app) << "Setting button size to:" << nBtnSize;
    if (m_backButton) {
        qCDebug(app) << "Setting back button size to:" << nBtnSize;
        m_backButton->setFixedSize(QSize(nBtnSize, nBtnSize));
    }
    if (m_forwardButton) {
        qCDebug(app) << "Setting forward button size to:" << nBtnSize;
        m_forwardButton->setFixedSize(QSize(nBtnSize, nBtnSize));
    }
    qCDebug(app) << "WebWindow::updateSizeMode() done.";
}

/**
 * @brief WebWindow::closeEvent
 * @param event
 * 重写关闭事件，保存窗口大小
 */
void WebWindow::closeEvent(QCloseEvent *event)
{
    qCDebug(app) << "Window close event received";

    //保存窗口大小
    saveWindowSize();
    QApplication::exit(0);
    qCDebug(app) << "Force exiting application";
    _Exit(0);
}

/**
 * @brief WebWindow::resizeEvent
 * @param event
 * 拖拽调整窗口大小时重新设置搜索结果位置
 */
void WebWindow::resizeEvent(QResizeEvent *event)
{
    qCDebug(app) << "Window resized to:" << event->size();

    Q_UNUSED(event);

    // 当窗口缩小时，搜索控件会被压缩，现调整为动态变化
    int detal = LIMIT_SEARCH_EDIT_WIDTH - event->size().width();
    if (detal <= 0) {
        search_edit_->setFixedWidth(SEARCH_EDIT_WIDTH);
    } else if (detal != LIMIT_SEARCH_EDIT_WIDTH) {
        search_edit_->setFixedWidth(SEARCH_EDIT_WIDTH - detal);
    }

    if (completion_window_->isVisible()) {
        qCDebug(app) << "Resizing completion window to:" << event->size();
        completion_window_->autoResize();
        // Move to below of search edit.
        const QPoint local_point(this->rect().width() / 2 - search_edit_->width() / 2,
                                 titlebar()->height() - 3);
        if (!Utils::judgeWayLand()) {
            const QPoint global_point(this->mapToGlobal(local_point));
            completion_window_->move(global_point);
            qCDebug(app) << "Resizing with global point:" << global_point;
        } else {
            completion_window_->move(local_point);
            qCDebug(app) << "Resizing with local point:" << local_point;
        }
    }

    if (web_view_) {
        qCDebug(app) << "Resizing web view to:" << event->size();
        slot_ThemeChanged();
    }
    qCDebug(app) << "WebWindow::resizeEvent() done.";
}

/**
 * @brief WebWindow::inputMethodEvent
 * @param e
 * 中文输入法支持
 */
void WebWindow::inputMethodEvent(QInputMethodEvent *e)
{
    qCDebug(app) << "WebWindow::inputMethodEvent()";
    if (!e->commitString().isEmpty() && search_edit_->lineEdit()->text() != e->commitString()) {
        qCDebug(app) << "Commit string:" << e->commitString();
        search_edit_->lineEdit()->setText(e->commitString());
        search_edit_->lineEdit()->setFocus();
    }
    QWidget::inputMethodEvent(e);
    qCDebug(app) << "WebWindow::inputMethodEvent() done.";
}

/**
 * @brief WebWindow::inputMethodQuery
 * @param prop
 * @return
 * 中文输入法支持
 */
QVariant WebWindow::inputMethodQuery(Qt::InputMethodQuery prop) const
{
    qCDebug(app) << "WebWindow::inputMethodQuery()";
    switch (prop) {
    case Qt::ImEnabled:
        qCDebug(app) << "Input method enabled";
        return true;
    default:
        qCDebug(app) << "Input method not enabled";
        break;
    }

    qCDebug(app) << "WebWindow::inputMethodQuery() done.";
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
    qCDebug(app) << "WebWindow::eventFilter()";
    //warland环境下watched的objectname不是QMainWindowClassWindow,先去除验证
    if (event->type() == QEvent::MouseButtonRelease && qApp->activeWindow() == this) {
        qCDebug(app) << "eventFilter mouse release";
        QRect rect = hasWidgetRect(search_edit_);
        if (web_view_ && web_view_->selectedText().isEmpty() && !rect.contains(QCursor::pos())) {
            qCDebug(app) << "set focus to web view";
            this->setFocus();
        }
    }
    if (event->type() == QEvent::KeyPress && qApp->activeWindow() == this) {
        qCDebug(app) << "eventFilter key press";
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_V
                && keyEvent->modifiers().testFlag(Qt::ControlModifier)) {
            const QString &clipboardText = QApplication::clipboard()->text();
            // support Ctrl+V shortcuts.
            if (!clipboardText.isEmpty()) {
                qCDebug(app) << "Ctrl+V, set text to search edit";
                search_edit_->lineEdit()->setFocus();
            }
        } else if (keyEvent->key() == Qt::Key_C
                   && keyEvent->modifiers().testFlag(Qt::ControlModifier)) {
            qCDebug(app) << "Ctrl+C, set text to clipboard";
            QApplication::clipboard()->setText(web_view_->selectedText());
        } else if (((keyEvent->key() <= Qt::Key_Z && keyEvent->key() >= Qt::Key_A)
                    || (keyEvent->key() <= Qt::Key_9 && keyEvent->key() >= Qt::Key_0)
                    || (keyEvent->key() == Qt::Key_Space))
                   && keyEvent->modifiers() == Qt::NoModifier) {
            qCDebug(app) << "Normal key, set focus to search edit";
            search_edit_->lineEdit()->setFocus();
        } else if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
            qCDebug(app) << "Enter key, set focus to search edit";
            if (search_edit_->lineEdit()->hasFocus()) {
                qCDebug(app) << "Enter key, set focus to search edit";
                //搜索框内容为空时，按回车键回到未搜索页面
                if (search_edit_->lineEdit()->text().isEmpty()) {
                    emit manual_proxy_->searchEditTextisEmpty();
                    qCDebug(app) << "emit searchEditTextisEmpty";
                }
            }
        } else if (keyEvent->key() == Qt::Key_A
                   && keyEvent->modifiers().testFlag(Qt::ControlModifier)) {
            qCDebug(app) << "Ctrl+A, select all text";
            //搜索框全选
            if (!search_edit_->lineEdit()->hasFocus()) {
                qCDebug(app) << "search line edit not has focus, set focus to web view";
                web_view_->setFocus(Qt::ActiveWindowFocusReason);
            }
        }
    }

    // Filters mouse press event only.
    if (event->type() == QEvent::MouseButtonPress && qApp->activeWindow() == this) {
        qCDebug(app) << "eventFilter mouse press";

        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (nullptr != mouseEvent) {
            switch (mouseEvent->button()) {
            case Qt::BackButton: {
                qCDebug(app) << "eventFilter back";
                emit title_bar_proxy_->backwardButtonClicked();
                break;
            }
            case Qt::ForwardButton: {
                qCDebug(app) << "eventFilter forward";
                emit title_bar_proxy_->forwardButtonClicked();
                break;
            }
            case Qt::MiddleButton:
                qCDebug(app) << "eventFilter middle";
                return true;
            default:
                break;
            }
        }

        //wayland 当搜索结果隐藏之后,将无法触发鼠标点击事件无法跳转内容
        if (!Utils::judgeWayLand()) {
            qCDebug(app) << "eventFilter middle button";
            if (!hasWidgetRect(search_edit_).contains(mapFromGlobal(QCursor::pos()))) {
                qCDebug(app) << "Middle button clicked outside search edit, hiding completion window";
                completion_window_->hide();
            }
        }
    }

    //过滤字体改变事件
    if (event->type() == QEvent::FontChange && watched == this) {
        qCDebug(app) << "eventFilter font change";
        if (this->settings_proxy_) {
            qCDebug(app) << "eventFilter QEvent::FontChange";
            QFontInfo fontInfo = this->fontInfo();
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
    qCDebug(app) << "setSearchManager";
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
    qCDebug(app) << "setSearchManager done.";
}

/**
 * @brief WebWindow::initConnections
 * 绑定信号槽
 */
void WebWindow::initConnections()
{
    qCDebug(app) << "Initializing connections";
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
#ifdef DTKWIDGET_CLASS_DSizeMode
    // 紧凑模式信号处理
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::sizeModeChanged, this, &WebWindow::updateSizeMode);
    updateSizeMode();
#endif
    qCDebug(app) << "Initializing connections done.";
}

/**
 * @brief WebWindow::onManualSearchByKeyword
 * @param keyword
 * 关键字搜索
 */
void WebWindow::onManualSearchByKeyword(const QString &keyword)
{
    qCDebug(app) << "WebWindow: onManualSearchByKeyword keyword:" << keyword;
    this->onSearchContentByKeyword(keyword);
}

/**
 * @brief WebWindow::onAppearanceChanged
 * @param map 个性化对, QString:更改类型  QVariant:更改后的值
 * @note 改变系统个性化时，触发此槽
 */
void WebWindow::onAppearanceChanged(QString, QMap<QString, QVariant> map, QStringList)
{
    qCDebug(app) << "onAppearanceChanged";
    if (image_viewer_)
        image_viewer_->hide();

    //20210630codereview
    if (map.isEmpty()) {
        qCDebug(app) << "onAppearanceChanged map is empty";
        return;
    }

    QString strValue = map.begin().value().toString();
    QString strKey = map.begin().key();
    qCDebug(app) << __func__ << " key: " << strKey << " value: " << strValue;
    if (0 == strKey.compare("QtActiveColor")) {
        qCDebug(app) << "onAppearanceChanged QtActiveColor";
        QTimer::singleShot(100, this, [&]() {
            //获取系统活动色
            QColor fillColor = DGuiApplicationHelper::instance()->applicationPalette().highlight().color();
            web_view_->page()->runJavaScript(QString("setHashWordColor('%1')").arg(fillColor.name()));
        });
    } else if (0 == strKey.compare("StandardFont")) {
        qCDebug(app) << "onAppearanceChanged StandardFont";
        //获取系统字体
        web_view_->page()->runJavaScript(QString("setWordFontfamily('%1')").arg(strValue));
    } else if (0 == strKey.compare("IconTheme") && nullptr != manual_proxy_) {
        qCDebug(app) << "onAppearanceChanged IconTheme";
        //系统图表主题变更
        emit manual_proxy_->iconThemeChanged(strValue);
    }
    qCDebug(app) << "onAppearanceChanged done.";
}

void WebWindow::onTimeoutLock(const QString &serviceName, QVariantMap key2value, QStringList)
{
    Q_UNUSED(serviceName);
    qCDebug(app) << "onTimeoutLock";
    if (key2value.value("Locked").value<bool>()) {
        qCDebug(app) << "System locked, hiding image viewer";
        if (image_viewer_)
            image_viewer_->hide();
    }
    qCDebug(app) << "onTimeoutLock done.";
}

/**
 * @brief WebWindow::initUI
 * @note 初始化窗口
 */
void WebWindow::initUI()
{
    qCDebug(app) << "Initializing UI components";

    //搜索结果框可移至主窗口创建完成后
    if (!Utils::judgeWayLand()) {
        qCDebug(app) << "Creating completion window";
        completion_window_ = new SearchCompletionWindow();
    } else {
        qCDebug(app) << "Creating completion window for Wayland";
        completion_window_ = new SearchCompletionWindow(this);
        completion_window_->hide();
    }

    qCDebug(app) << "Setting focus for active window";
    setFocus(Qt::ActiveWindowFocusReason);
    // 初始化标题栏
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setContentsMargins(0, 0, 0, 0);
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
        qCDebug(app) << "Support Selper";
        DMenu *pMenu = new DMenu;
        QAction *pHelpSupport = new QAction(QObject::tr("Support"));
        pMenu->addAction(pHelpSupport);
        this->titlebar()->setMenu(pMenu);
        connect(pHelpSupport, &QAction::triggered, this, [this] {
            this->HelpSupportTriggered(true);
        });
    }
    this->titlebar()->addWidget(buttonFrame, Qt::AlignLeft);
    this->titlebar()->addWidget(search_edit_, Qt::AlignCenter);
    this->titlebar()->setSeparatorVisible(false);
    this->titlebar()->setIcon(QIcon::fromTheme("deepin-manual"));

    if (Utils::judgeWayLand()) {
        qCDebug(app) << "Wayland detected, setting titlebar background role";
        this->titlebar()->setAutoFillBackground(false);
        this->titlebar()->setBackgroundRole(QPalette::Window);
    }
    //隐藏title阴影
    this->setTitlebarShadowEnabled(false);
    //键盘盲打
    // search_edit_->setFocus();
    this->setFocusPolicy(Qt::ClickFocus);

    // 创建QStackedWidget
    m_CentralStackWidget = new QStackedWidget(this);
    
    // 创建spinner页面
    QWidget *spinnerPage = new QWidget;
    QVBoxLayout *spinnerLayout = new QVBoxLayout(spinnerPage);
    m_spinner->setFixedSize(50, 50);
    spinnerLayout->addWidget(m_spinner, 0, Qt::AlignCenter);
    m_spinner->start();

    // 添加spinner页面到stackWidget
    m_CentralStackWidget->addWidget(spinnerPage);
    
    // 设置stackWidget为中央部件
    this->setCentralWidget(m_CentralStackWidget);
    qCDebug(app) << "WebWindow initialized";
}

/**
 * @brief WebWindow::initWebView
 * @note 初始化qwebengines, 初始化QWebChannel,通过QWebChannel完成QT与JS间通讯
 */
void WebWindow::initWebView()
{
    qCDebug(app) << "Initializing WebView components...";

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
#ifdef QT_DEBUG
    // 创建开发者工具窗口并设置
    devToolsView = new QWebEngineView();
    devToolsView->setWindowTitle("Developer Tools - deepin-manual");
    devToolsView->resize(1200, 800);
    web_view_->page()->setDevToolsPage(devToolsView->page());
    
    // 添加快捷键F12打开开发者工具
    QShortcut *devToolsShortcut = new QShortcut(QKeySequence::fromString("F12"), this);
    connect(devToolsShortcut, &QShortcut::activated, [this]() {
        if (devToolsView && devToolsView->isVisible()) {
            devToolsView->hide();
        } else if (devToolsView) {
            devToolsView->show();
            devToolsView->raise();
        }
    });
#endif // QT_DEBUG
    // 添加web_view_页面到stackWidget
    m_CentralStackWidget->addWidget(web_view_);

    if (!Utils::judgeWayLand()) {
        qCDebug(app) << "Setting WA_NativeWindow attribute for non-Wayland environment";
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
    web_channel->registerObject(QStringLiteral("i18n"), i18n_proxy);
    web_channel->registerObject(QStringLiteral("imageViewer"), image_viewer_proxy_);
    web_channel->registerObject(QStringLiteral("manual"), manual_proxy_);
    web_channel->registerObject(QStringLiteral("search"), search_proxy_);
    web_channel->registerObject(QStringLiteral("theme"), theme_proxy_);
    web_channel->registerObject(QStringLiteral("titleBar"), title_bar_proxy_);
    web_channel->registerObject(QStringLiteral("settings"), settings_proxy_);
    web_view_->page()->setWebChannel(web_channel);

    connect(web_view_->page(), &QWebEnginePage::loadProgress, this, &WebWindow::onWebPageLoadProgress);
    connect(manual_proxy_, &ManualProxy::channelInit, this, &WebWindow::onChannelFinish);
    connect(manual_proxy_, &ManualProxy::WidgetLower, this, &WebWindow::lower);
    connect(manual_proxy_, &ManualProxy::updateLabel, this, &WebWindow::slotUpdateLabel);
    connect(manual_proxy_, &ManualProxy::supportBeClick, this, [this] { this->HelpSupportTriggered(); });
    connect(manual_proxy_, &ManualProxy::appStoreBeClick, this, [this] { this->appStoreTriggered(); });

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
    qCDebug(app) << "WebView components initialized";
}

/**
 * @brief WebWindow::saveWindowSize 记录最后一个正常窗口的大小
 */
void WebWindow::saveWindowSize()
{
    qCDebug(app) << "Saving window size";
    QSettings *setting = ConfigManager::getInstance()->getSettings();
    setting->beginGroup(QString(kConfigWindowInfo));
    setting->setValue(QString(kConfigWindowWidth), width());
    setting->setValue(QString(kConfigWindowHeight), height());
    setting->endGroup();
    qCDebug(app) << "Window size saved";
}
/**
 * @brief WebWindow::updateDb 更新数据库
 */
void WebWindow::updateDb()
{
    qCDebug(app) << "Updating database";
    if (!search_manager_) {
        qCDebug(app) << "search_manager_ is null";
        return;
    }

    //更新数据库
    emit search_manager_->updateDb();
    qCDebug(app) << "Database updated";
}

/**
 * @brief WebWindow::initShortcuts 设置窗口支持的快捷键
 */
void WebWindow::initShortcuts()
{
    qCDebug(app) << "init Short cuts";
    //设置窗口大小切换快捷键
    QShortcut *scWndReize = new QShortcut(this);
    scWndReize->setKey(tr("Ctrl+Alt+F"));
    scWndReize->setContext(Qt::WindowShortcut);
    scWndReize->setAutoRepeat(false);
    connect(scWndReize, &QShortcut::activated, this, [this] {
        if (this->windowState() & Qt::WindowMaximized)
        {
            qCDebug(app) << "Window is maximized, restore it";
            this->showNormal();
        } else if (this->windowState() == Qt::WindowNoState)
        {
            qCDebug(app) << "Window is normal, maximize it";
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
    qCDebug(app) << "init Short cuts done";
}

/**
 * @brief WebWindow::initDBus 初始化Dbus接口, 获取系统设置中个性化设置变化信号.
 */
void WebWindow::initDBus()
{
    qCDebug(app) << "Initializing DBus connections";

    QString appearanceService = "com.deepin.daemon.Appearance";
    QString appearancePath = "/com/deepin/daemon/Appearance";
    if (DSysInfo::majorVersion().toInt() >= 23) {
        appearanceService = "org.deepin.dde.Appearance1";
        appearancePath = "/org/deepin/dde/Appearance1";
    }
    QDBusConnection senderConn = QDBusConnection::connectToBus(QDBusConnection::SessionBus, "Sender");
    if (!senderConn.connect(
                appearanceService, // sender's service name
                appearancePath, // sender's path name
                "org.freedesktop.DBus.Properties", // interface
                "PropertiesChanged", // sender's signal name
                this, // receiver
                SLOT(onAppearanceChanged(QString, QMap<QString, QVariant>, QStringList)))) {
        qCWarning(app) << "connectToBus()::connect()  PropertiesChanged failed";
    } else {
        qCDebug(app) << "connectToBus()::connect()  PropertiesChanged success";
    }

    // 锁屏通知，显示隐藏图片界面，才能保证图片界面正常隐藏，否则图片界面会一直阻塞主线程鼠标事件传递
    QDBusConnection::sessionBus().connect("com.deepin.SessionManager", "/com/deepin/SessionManager",
                                          "org.freedesktop.DBus.Properties", "PropertiesChanged", this,
                                          SLOT(onTimeoutLock(QString, QVariantMap, QStringList)));
    qCDebug(app) << "Initializing DBus connections done";
}

/**
 * @brief WebWindow::setHashWordColor
 * 调用ｊｓ接口，改变活动色
 */
void WebWindow::setHashWordColor()
{
    qCDebug(app) << "Setting hash word color";
    QColor Color = DGuiApplicationHelper::instance()->applicationPalette().highlight().color();
    QString strColor = Color.name(QColor::NameFormat::HexRgb);
    web_view_->page()->runJavaScript(QString("setHashWordColor('%1')").arg(strColor));
    completion_window_->updateColor(Color);
    qCDebug(app) << "Hash word color set";
}

/**
 * @brief WebWindow::settingContextMenu
 * 初始化鼠标选中内容右键菜单Menu.
 */
void WebWindow::settingContextMenu()
{
    qCDebug(app) << "Setting context menu for web view";
    web_view_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(web_view_, &QWebEngineView::selectionChanged, this, [this]() {
        web_view_->setContextMenuPolicy(Qt::CustomContextMenu);
    });
    QMenu *menu = new QMenu(this);
    //文案内容右键显示复制
    QAction *action = menu->addAction(QObject::tr("Copy"));
    connect(web_view_, &QWidget::customContextMenuRequested, this, [ = ]() {
        if (!web_view_->selectedText().isEmpty()) {
            qCDebug(app) << "Copy selected text";
            menu->exec(QCursor::pos());
        }
    });
    connect(action, &QAction::triggered, this, [ = ]() {
        QApplication::clipboard()->setText(web_view_->selectedText());
    });
    qCDebug(app) << "Context menu for web view set";
}

/**
 * @brief WebWindow::hasWidgetRect
 * @return 返回控件Rect位置
 * 根据父控件位置确定子控件位置
 */
QRect WebWindow::hasWidgetRect(QWidget *widget)
{
    qCDebug(app) << "has widget rect";
    QRect rect;
    QWidget *tmpWidget = widget;
    while (tmpWidget) {
        rect.setX(rect.x() + tmpWidget->x());
        rect.setY(rect.y() + tmpWidget->y());
        tmpWidget = tmpWidget->parentWidget();
    }
    rect.setWidth(widget->width());
    rect.setHeight(widget->height());
    qCDebug(app) << "widget rect:" << rect;
    return rect;
}

/**
 * @brief WebWindow::onSearchContentByKeyword
 * @param keyword 搜索关键字
 * @note 通过关键字进行搜索，显示探索页面
 */
void WebWindow::onSearchContentByKeyword(const QString &keyword)
{
    qCDebug(app) << "calling keyword is:" << keyword;
    QString key(keyword);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QRegExp sreg("\\s");
#else
    QRegularExpression sreg("\\s");
#endif
    const QString searchKey = key.remove('\n').remove('\r').remove("\r\n").remove(sreg);
    //在数据库中查询->SearchDb::searchContent->SearchDb::handleSearchContent
    search_manager_->searchContent(searchKey);

    QString base64Key = QString(searchKey.toUtf8().toBase64());
    qCDebug(app) << " base64Key " << base64Key;

    // 调用ｊｓ接口显示搜索内容
    web_view_->page()->runJavaScript(QString("openSearchPage('%1')").arg(base64Key));
    qCDebug(app) << "Search content by keyword done";
}

/**
 * @brief WebWindow::onSearchEditFocusOut
 * 搜索框失去焦点
 */
void WebWindow::onSearchEditFocusOut()
{
    qCDebug(app) << "onSearchEditFocusOut";
    QTimer::singleShot(20, this, [ = ]() {
        this->completion_window_->hide();
    });
    qCDebug(app) << "onSearchEditFocusOut done";
}

/**
 * @brief WebWindow::onSearchButtonClicked
 * @note 输入搜索关键字之后点击弹出窗口中的最下部的 SearchButton触发
 */
void WebWindow::onSearchButtonClicked()
{
    qCDebug(app) << "onSearchButtonClicked";
    QString textTemp = search_edit_->text();
    const QString text = textTemp.remove('\n').remove('\r').remove("\r\n");
    //根据关键字在内容中查找
    this->onSearchContentByKeyword(text);
    completion_window_->hide();
    qCDebug(app) << "onSearchButtonClicked done";
}

/**
 * @brief WebWindow::onSearchResultClicked
 * @param result
 * @note 点击SearchList Item 时调用js方法
 */
void WebWindow::onSearchResultClicked(const SearchAnchorResult &result)
{
    qCDebug(app) << "onSearchResultClicked";
    web_view_->page()->runJavaScript(QString("open('%1', '%2', '%3')").arg(result.app_name, result.anchorId, result.anchor));
    qCDebug(app) << "onSearchResultClicked done";
}

/**
 * @brief WebWindow::onSearchTextChanged
 * @param text
 * @note ２００ｍｓ内搜索框中的文本改变时重新计时执行一次搜索操作, 200ms搜索一次
 */
void WebWindow::onSearchTextChanged(const QString &text)
{
    qCDebug(app) << "onSearchTextChanged";
    if (bIsSetKeyword) {
        qCDebug(app) << "onSearchTextChanged is set keyword";
        bIsSetKeyword = false;
    } else if (text.size() >= 1) {
        qCDebug(app) << "onSearchTextChanged is text size >= 1";
        search_timer_.stop();
        search_timer_.start(kSearchDelay);
    } else {
        qCDebug(app) << "onSearchTextChanged is text size < 1";
        this->onSearchEditFocusOut();
    }
    qCDebug(app) << "onSearchTextChanged done";
}

/**
 * @brief WebWindow::onSearchTextChangedDelay
 * 获取searchEidt中的文本，过滤特殊字符与中隔，设置searchButton显示文本
 */
void WebWindow::onSearchTextChangedDelay()
{
    qCDebug(app) << "onSearchTextChangedDelay";
    QString textTemp = search_edit_->text();
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QRegExp sreg("\\s");
    QRegExp ereg("[+-_$!@#%^&\\(\\)]");
#else
    QRegularExpression sreg("\\s");
    QRegularExpression ereg("[+-_$!@#%^&\\(\\)]");
#endif
    const QString text = textTemp.remove('\n').remove('\r').remove("\r\n").remove(sreg);
    // 过滤特殊字符
    if (text.size() < 1 || text.toLower().contains(ereg)) {
        qCDebug(app) << "onSearchTextChangedDelay is text size < 1 or contains special characters";
        return;
    }
    completion_window_->setKeyword(text);

    //执行搜索
    if (search_manager_) {
        qCDebug(app) << "onSearchTextChangedDelay emit search_manager_->searchAnchor";
        emit search_manager_->searchAnchor(text);
    }
    qCDebug(app) << "onSearchTextChangedDelay done";
}

/**
 * @brief WebWindow::onTitleBarEntered
 * 焦点在SearchEdit中，按回车时调用 ，调用onEnterPressed()进行搜索操作
 */
void WebWindow::onTitleBarEntered()
{
    qCDebug(app) << "onTitleBarEntered";

    QString textTemp = search_edit_->text();
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QRegExp sreg("\\s");
#else
    QRegularExpression sreg("\\s");
#endif
    const QString text = textTemp.remove('\n').remove('\r').remove("\r\n").remove(sreg);
    if (text.size() >= 1) {
        qCDebug(app) << "onTitleBarEntered is text size >= 1";
        completion_window_->onEnterPressed();
    }
    qCDebug(app) << "onTitleBarEntered done";
}

/**
 * @brief WebWindow::onWebPageLoadProcess qwebengines加载进度响应槽
 * @valpro  网页加载进度响应
 * 网页加载达到20%就显示提什用户体验
 */
void WebWindow::onWebPageLoadProgress(int valpro)
{
    qCDebug(app) << "Web page loading progress:" << valpro << "%";

    if (m_spinner->isPlaying() && valpro > 20) {
        qCDebug(app) << "Page loading reached 20%, hiding spinner";
        m_spinner->stop();
        m_spinner->hide();
        disconnect(web_view_->page(), &QWebEnginePage::loadProgress, this, &WebWindow::onWebPageLoadProgress);
        
        // 切换到web_view_页面
        m_CentralStackWidget->setCurrentWidget(web_view_);
        qCDebug(app) << "Switched to web view widget";
    }
    qCDebug(app) << "onWebPageLoadProgress done";
}

/**
 * @brief WebWindow::onChannelFinish
 * 完成channel对象与Qt对象绑定后回调
 */
void WebWindow::onChannelFinish()
{
    qCDebug(app) << "Web channel initialization completed";

    bFinishChannel = true;
    setHashWordColor();
    settingContextMenu();
    //设置主题
    QString qsthemetype = "Null";
    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    if (themeType == DGuiApplicationHelper::LightType) {
        qCDebug(app) << "Theme type is light";
        qsthemetype = "LightType";
    } else if (themeType == DGuiApplicationHelper::DarkType) {
        qCDebug(app) << "Theme type is dark";
        qsthemetype = "DarkType";
    }
    qCDebug(app) << "Theme type:" << qsthemetype;
    web_view_->page()->runJavaScript(QString("setTheme('%1')").arg(qsthemetype));
    //设置打开页面
    if (app_name_.isEmpty()) {
        qCDebug(app) << "Open index page";
        web_view_->page()->runJavaScript("index()");
    } else {
        qCDebug(app) << "Open app page";
        QString real_path(app_name_);
        if (real_path.contains('/')) {
            qCDebug(app) << "Open markdown file with absolute path";
            // Open markdown file with absolute path.
            QFileInfo info(real_path);
            real_path = info.canonicalFilePath();
            web_view_->page()->runJavaScript(QString("open('%1')").arg(real_path));
        } else {
            qCDebug(app) << "Open app page with title";
            web_view_->page()->runJavaScript(QString("openTitle('%1','%2')").arg(app_name_, title_name_));
        }
    }
    if (first_webpage_loaded_) {
        qCDebug(app) << "First webpage loaded";
        first_webpage_loaded_ = false;
        if (keyword_.length() > 0) {
            qCDebug(app) << "Emit manualSearchByKeyword signal";
            emit this->manualSearchByKeyword(keyword_);
        }
    }
    //设置字体
    if (this->settings_proxy_) {
        qCDebug(app) << "Emit fontChangeRequested signal";
        QFontInfo fontInfo = this->fontInfo();
        emit this->settings_proxy_->fontChangeRequested(fontInfo.family(),
                                                        fontInfo.pixelSize());
    }
    qCDebug(app) << "onChannelFinish done";
}

/**
 * @brief WebWindow::onSetKeyword JS根据页面关键字回调设置搜索框
 * @param keyword 关键字
 * @note keyword 可能存在转换后的字符,所以需要将字符反向转换回去.
 */
void WebWindow::onSetKeyword(const QString &keyword)
{
    qCDebug(app) << "onSetKeyword";
    bIsSetKeyword = true;
    QTimer::singleShot(40, [ = ]() {
        bIsSetKeyword = false;
    });

    if (search_edit_) {
        qCDebug(app) << "onSetKeyword is set keyword";
        if (keyword.isEmpty()) {
            qCDebug(app) << "onSetKeyword is empty keyword";
            //清空搜索框
            search_edit_->clearEdit();
        } else {
            QString strTemp = keyword;
            if (strTemp.contains("=-=")) {
                qCDebug(app) << "onSetKeyword is contains '=-='";
                strTemp.replace("=-=", "%");
            }
            //设置搜索框值
            search_edit_->setText(strTemp);
        }
    }
    qCDebug(app) << "onSetKeyword done";
}

/**
 * @brief WebWindow::onSearchAnchorResult 搜索结果框内容显示
 * @param keyword 搜索关键字
 * @param result  搜索的内容
 */
void WebWindow::onSearchAnchorResult(const QString &keyword, const SearchAnchorResultList &result)
{
    qCDebug(app) << "Received search anchor results for keyword:" << keyword;

    //搜索结果不存在，忽略这个信号
    if (keyword != completion_window_->keyword()) {
        qCDebug(app) << "onSearchAnchorResult keyword not equal";
        return;
    }

    if (result.isEmpty()) {
        qCDebug(app) << "onSearchAnchorResult result is empty";
        // Hide completion window if no anchor entry matches.
        completion_window_->hide();
    } else {
        qCDebug(app) << "onSearchAnchorResult result is not empty";
        completion_window_->setSearchAnchorResult(result);
        completion_window_->show();
        completion_window_->raise();
        completion_window_->autoResize();
        // Move to below of search edit.
        const QPoint local_point(this->rect().width() / 2 - search_edit_->width() / 2,
                                 titlebar()->height() - 3);
        if (!Utils::judgeWayLand()) {
            qCDebug(app) << "onSearchAnchorResult is not landscape";
            const QPoint global_point(this->mapToGlobal(local_point));
            completion_window_->move(global_point);
        } else {
            qCDebug(app) << "onSearchAnchorResult is landscape";
            completion_window_->move(local_point);
        }

        completion_window_->setFocusPolicy(Qt::NoFocus);
        completion_window_->setFocusPolicy(Qt::StrongFocus);
    }
    qCDebug(app) << "onSearchAnchorResult done";
}
