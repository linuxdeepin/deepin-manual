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

#ifndef DEEPIN_MANUAL_VIEW_WEB_WINDOW_H
#define DEEPIN_MANUAL_VIEW_WEB_WINDOW_H

#include "controller/search_result.h"

#include <DButtonBox>
#include <DMainWindow>
#include <DApplicationHelper>
#include <DSpinner>
#include <DMessageManager>

#include <QtDBus/QtDBus>
#include <QWebEngineView>

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

    Dtk::Widget::DButtonBoxButton *m_backButton;
    Dtk::Widget::DButtonBoxButton *m_forwardButton;

signals:
    void manualSearchByKeyword(const QString &keyword);
    void manualStartFinish(qint64);

public slots:
    void slot_ThemeChanged();
    void slotUpdateLabel();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void inputMethodEvent(QInputMethodEvent *e) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    QVariant inputMethodQuery(Qt::InputMethodQuery prop) const Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

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
    QTimer search_timer_ {nullptr};
    Dtk::Widget::DButtonBox *buttonBox {nullptr};
    SearchEdit *search_edit_ {nullptr};
    bool first_webpage_loaded_ {true};
    bool bIsSetKeyword{false};
    bool bFinishChannel{false};
    Dtk::Widget::DSpinner *m_spinner;

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
    //    void onThemeChange(DGuiApplicationHelper::ColorType themeType);
};

#endif // DEEPIN_MANUAL_VIEW_WEB_WINDOW_H
