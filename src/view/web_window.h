// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEEPIN_MANUAL_VIEW_WEB_WINDOW_H
#define DEEPIN_MANUAL_VIEW_WEB_WINDOW_H

#include "controller/search_result.h"

#include <DButtonBox>
#include <DMainWindow>
#include <DGuiApplicationHelper>
#include <DSpinner>
#include <DMessageManager>

#include <QtDBus/QtDBus>
#include <QWebEngineView>
#include <QStackedWidget>

DWIDGET_USE_NAMESPACE

class I18nProxy;
class ImageViewer;
class ImageViewerProxy;
class ManualProxy;
class SearchCompletionWindow;
class SearchManager;
class SearchProxy;
class ThemeProxy;
class TitleBar;
class TitleBarProxy;
class SettingsProxy;
class SearchEdit;
class DButtonBox;

/**
 * @brief The WebWindow class
 * 帮助手册主窗口类，继承
 */
class WebWindow : public Dtk::Widget::DMainWindow
{
    Q_OBJECT

public:
    explicit WebWindow(QWidget *parent = nullptr);
    ~WebWindow() override;

    void initWeb();
    void updatePage(const QStringList &list);
    void updateBtnBox();
    void cancelTextChanged();
    void openjsPage(const QString &app_name, const QString &title_name);
    void setAppProperty(const QString &appName, const QString &titleName, const QString &keyword);
    void saveWindowSize();
    void updateDb();

    Dtk::Widget::DButtonBoxButton *m_backButton;
    Dtk::Widget::DButtonBoxButton *m_forwardButton;

signals:
    void manualSearchByKeyword(const QString &keyword);
    void manualStartFinish(qint64);

public slots:
    void slot_ThemeChanged();
    void slotUpdateLabel();

private slots:
    /**
     * @brief 根据布局模式(紧凑)变更更新界面布局
     */
    void updateSizeMode();
protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void inputMethodEvent(QInputMethodEvent *e) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) override;
    QVariant inputMethodQuery(Qt::InputMethodQuery prop) const Q_DECL_OVERRIDE;

private:
    void initUI();
    void initDBus();
    void initWebView();
    void initShortcuts();
    void initConnections();
    void setHashWordColor();
    void setSearchManager();
    void settingContextMenu();
    QRect hasWidgetRect(QWidget *);
    void HelpSupportTriggered(bool bActiontrigger = false);
    void appStoreTriggered();

    QString app_name_;
    QString keyword_;
    QString title_name_;
    SearchManager *search_manager_ {nullptr};
    SearchCompletionWindow *completion_window_ {nullptr};
    ImageViewer *image_viewer_ {nullptr};
    I18nProxy *i18n_proxy {nullptr};
    SearchProxy *search_proxy_ {nullptr};
    ThemeProxy *theme_proxy_ {nullptr};
    ImageViewerProxy *image_viewer_proxy_ {nullptr};
    ManualProxy *manual_proxy_ {nullptr};
    TitleBarProxy *title_bar_proxy_ {nullptr};
    SettingsProxy *settings_proxy_ {nullptr};
    TitleBar *title_bar_ {nullptr};
    QWebEngineView *web_view_ {nullptr};
    QWebEngineView *devToolsView {nullptr};
    QTimer search_timer_ {nullptr};
    Dtk::Widget::DButtonBox *buttonBox {nullptr};
    SearchEdit *search_edit_ {nullptr};
    bool first_webpage_loaded_ {true};
    bool bIsSetKeyword{false};
    bool bFinishChannel{false};
    Dtk::Widget::DSpinner *m_spinner;
    QStackedWidget *m_CentralStackWidget;

private slots:
    void onSearchEditFocusOut();
    void onSearchButtonClicked();
    void onSearchContentByKeyword(const QString &keyword);
    void onSearchResultClicked(const SearchAnchorResult &result);
    void onSearchAnchorResult(const QString &keyword, const SearchAnchorResultList &result);
    void onSearchTextChanged(const QString &text);
    void onSearchTextChangedDelay();
    void onTitleBarEntered();
    void onWebPageLoadProgress(int valpro);
    void onChannelFinish();
    void onSetKeyword(const QString &keyword);
    void onManualSearchByKeyword(const QString &keyword);
    void onAppearanceChanged(QString, QMap<QString, QVariant>, QStringList);
    // 是否锁屏
    void onTimeoutLock(const QString &, QVariantMap, QStringList);
};

#endif // DEEPIN_MANUAL_VIEW_WEB_WINDOW_H
