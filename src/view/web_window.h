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
#include "view/settings_proxy.h"

#include <QTimer>
#include <QtDBus/QtDBus>

#include <DButtonBox>
#include <DMainWindow>
#include <QWebEngineView>
#include <QAction>
#include <QClipboard>

namespace dman {

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


class WebWindow : public Dtk::Widget::DMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QString appName READ appName WRITE setAppName)

public:
    explicit WebWindow(QWidget *parent = nullptr);
    ~WebWindow() override;

    // Get app name of manual currently presented.
    const QString &appName() const;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void setSearchManager(SearchManager *searchManager);
    void initWebView();
    void setTitleName(const QString &title_name);
    void cancelTextChanged();
    void updateBtnBox();

    // 保存窗口尺寸
    void saveWindowSize();

    Dtk::Widget::DButtonBoxButton *m_backButton;
    Dtk::Widget::DButtonBoxButton *m_forwardButton;

signals:
    void closed(const QString &app_name);
    void shown(WebWindow *window);
    void manualSearchByKeyword(const QString &keyword);
    void webWindowShown();
    void webWindowLoaded();

public slots:
    void setAppName(const QString &app_name);
    void setSearchKeyword(const QString &keyword);
    void slot_ButtonHide();
    void slot_ButtonShow();
    void slot_ThemeChanged();

protected:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void inputMethodEvent(QInputMethodEvent *e) Q_DECL_OVERRIDE;
    QVariant inputMethodQuery(Qt::InputMethodQuery prop) const Q_DECL_OVERRIDE;

private:
    void initConnections();
    void initUI();
    void initShortcuts();
    void initDBus();
    void setHashWordColor();
    void settingContextMenu();

    QString app_name_;
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
    QTimer search_timer_;
    Dtk::Widget::DButtonBox *buttonBox {nullptr};
    SearchEdit *search_edit_ {nullptr};
    QPoint start_point_;
    int start_drag_x;
    QString keyword_;
    bool first_webpage_loaded_ {true};
    bool is_index_loaded_ {false};

private slots:
    void onSearchEditFocusOut();
    void onSearchButtonClicked();
    void onSearchContentByKeyword(const QString &keyword);
    void onSearchResultClicked(const SearchAnchorResult &result);
    void onSearchAnchorResult(const QString &keyword, const SearchAnchorResultList &result);
    void onSearchTextChanged(const QString &text);
    void onSearchTextChangedDelay();
    void onTitleBarEntered();
    void onWebPageLoadFinished(bool ok);

    void onManualSearchByKeyword(const QString &keyword);
    void onACtiveColorChanged(QString, QMap<QString, QVariant>, QStringList);
};

}  // namespace dman

#endif  // DEEPIN_MANUAL_VIEW_WEB_WINDOW_H
