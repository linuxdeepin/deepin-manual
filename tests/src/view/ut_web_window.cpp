/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     wangmingliang <wangmingliang@uniontech.com>
* Maintainer: wangmingliang <wanmgmingliang@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ut_web_window.h"

#include "view/web_window.h"
#include "view/widget/search_edit.h"
#include "view/widget/search_completion_window.h"
#include "view/widget/search_button.h"
#include "view/widget/image_viewer.h"
#include "controller/search_manager.h"
#include "controller/search_result.h"
#include "controller/config_manager.h"
#include "base/consts.h"
#include "view/i18n_proxy.h"
#include "view/image_viewer_proxy.h"
#include "view/manual_proxy.h"
#include "view/search_proxy.h"
#include "view/settings_proxy.h"
#include "view/theme_proxy.h"
#include "view/title_bar_proxy.h"

#include "../third-party/stub/stub.h"

namespace dman {

ut_web_window_test::ut_web_window_test()
{
}

void stub_inintweb_rebutn()
{
    return;
}
void ut_web_window_test::SetUp()
{
    Stub st;
    st.set(ADDR(WebWindow, initWeb), stub_inintweb_rebutn);
    window = new WebWindow;
}

void ut_web_window_test::TearDown()
{
    delete window;
}
TEST_F(ut_web_window_test, deleteWin)
{
    window->image_viewer_ = new ImageViewer();
    window->image_viewer_proxy_ = new ImageViewerProxy(window->image_viewer_);
    window->search_proxy_ = new SearchProxy;
    window->theme_proxy_ = new ThemeProxy;
    window->manual_proxy_ = new ManualProxy;
    window->title_bar_proxy_ = new TitleBarProxy;
    window->settings_proxy_ = new SettingsProxy;
    window->search_manager_ = new SearchManager;
    window->i18n_proxy = new I18nProxy;
    window->buttonBox = new Dtk::Widget::DButtonBox;
    window->search_edit_ = new SearchEdit;
    window->web_view_ = new QWebEngineView;
}

TEST_F(ut_web_window_test, updateBtnBox)
{
    WebWindow web;
    web.m_backButton->setEnabled(true);
    web.m_forwardButton->setEnabled(false);
    web.updateBtnBox();
    ASSERT_TRUE(web.buttonBox->isEnabled());
    web.m_backButton->setEnabled(false);
    web.m_forwardButton->setEnabled(false);
    web.updateBtnBox();
    ASSERT_FALSE(web.buttonBox->isVisible());
}

TEST_F(ut_web_window_test, openjsPage)
{
//    WebWindow web;
//    web.initWeb();
//    web.openjsPage("deepin-terminal", "查找");
}


TEST_F(ut_web_window_test, updatePage)
{
    WebWindow web;
    web.initUI();
    QStringList applistmd;
    applistmd.append("/usr/share/deepin-manual/manual-assets/application/dde-calendar/calendar/zh_HK/calendar.md");
    applistmd.append("/usr/share/deepin-manual/manual-assets/application/deepin-terminal/terminal/zh_CN/terminal.md");
    web.updatePage(applistmd);
}


TEST_F(ut_web_window_test, slot_HelpSupportTriggered)
{
    WebWindow web;
    web.slot_HelpSupportTriggered();
}


TEST_F(ut_web_window_test, slotUpdateLabel)
{
    WebWindow web;
    web.slotUpdateLabel();
}

TEST_F(ut_web_window_test, closeEvent)
{
        WebWindow web;
        web.initUI();
        //web.initWeb();
        web.setFixedWidth(600);
        web.setFixedHeight(1200);
        web.close();
        QSettings *setting = ConfigManager::getInstance()->getSettings();
        setting->beginGroup(kConfigWindowInfo);
        ASSERT_EQ(setting->value(kConfigWindowWidth), 600);
        ASSERT_EQ(setting->value(kConfigWindowHeight), 1200);
}



TEST_F(ut_web_window_test, inputMethodQuery)
{
    WebWindow web;
    Qt::InputMethodQuery prop;
    prop = Qt::ImEnabled;
    ASSERT_TRUE(web.inputMethodQuery(prop).toBool());
}

TEST_F(ut_web_window_test, eventFilter)
{
        WebWindow web;
        web.setObjectName("QMainWindowClassWindow");
        web.initUI();
        web.setFocus();
        qApp->setActiveWindow(&web);
        //web.search_edit_->lineEdit()->setFocus();
        QTest::keyPress(&web, Qt::Key_1);
        ASSERT_TRUE(web.search_edit_->lineEdit()->hasFocus());
}

TEST_F(ut_web_window_test, onManualSearchByKeyword)
{


}

TEST_F(ut_web_window_test, onAppearanceChanged)
{
    QMap<QString, QVariant> map;
    QVariant var = "#00A48A";
    map.insert("QtActiveColor", var);
    WebWindow web;
    web.onAppearanceChanged("", map, QStringList());
}


TEST_F(ut_web_window_test, saveWindowSize)
{
    WebWindow web;
    web.initUI();
    //web.initWeb();
    web.setFixedWidth(600);
    web.setFixedHeight(1200);
    web.saveWindowSize();
    QSettings *setting = ConfigManager::getInstance()->getSettings();

    setting->beginGroup(kConfigWindowInfo);
    ASSERT_EQ(setting->value(kConfigWindowWidth), 600);
    ASSERT_EQ(setting->value(kConfigWindowHeight), 1200);
    setting->endGroup();
}



TEST_F(ut_web_window_test, onSearchTextChanged)
{
        WebWindow web;
        web.initUI();
        web.onSearchTextChanged("应用");
        usleep(200000);
        ASSERT_FALSE(web.completion_window_->isVisible());
}


TEST_F(ut_web_window_test, onSearchTextChangedDelay)
{
        WebWindow web;
        web.initUI();
        web.search_edit_->setText("关闭应用商店");
        web.onSearchTextChangedDelay();
        ASSERT_EQ(web.completion_window_->keyword(), "关闭应用商店");
}
TEST_F(ut_web_window_test, onTitleBarEntered)
{
    WebWindow web;
    web.initUI();
    web.search_edit_->setText("关闭应用商店");
    web.onTitleBarEntered();
    ASSERT_FALSE(web.completion_window_->isVisible());
}

TEST_F(ut_web_window_test, onSetKeyword)
{
    WebWindow web;
    web.onSetKeyword("应用");
    ASSERT_EQ(web.search_edit_->text(), "应用");
}
TEST_F(ut_web_window_test, onSearchAnchorResult)
{
    WebWindow web;
    SearchAnchorResultList list;
    SearchAnchorResult result;
    result.anchor = "应用商店";
    result.anchorId = "h0";
    result.app_name = "deepin-app-store";
    result.app_display_name = "应用商店";
    list.append(result);
    SearchAnchorResult result2;
    result2.anchor = "运行应用商店";
    result2.anchorId = "h3";
    result2.app_name = "deepin-app-store";
    result2.app_display_name = "应用商店";
    list.append(result2);
    ASSERT_FALSE(web.completion_window_->isVisible());
    web.onSearchAnchorResult("", list);
    ASSERT_TRUE(web.completion_window_->isVisible());
    ASSERT_EQ(web.completion_window_->result_view_->count(), 2);
}
TEST_F(ut_web_window_test, onSearchAnchorResult2)
{
    WebWindow web;
    SearchAnchorResultList list;
    SearchAnchorResult result;

    web.onSearchAnchorResult("", list);
    ASSERT_FALSE(web.completion_window_->isVisible());
}

} // namespace dman
