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
#include "base/utils.h"
#include "../third-party/stub/stub.h"
#include <QClipboard>

QWebChannel * webchannel;
ut_web_window_test::ut_web_window_test()
{
}

void stub_inintweb_rebutn()
{
    return;
}


TEST_F(ut_web_window_test, updateBtnBox)
{
    WebWindow *web = new WebWindow;
    web->m_backButton->setEnabled(true);
    web->m_forwardButton->setEnabled(false);
    web->updateBtnBox();
    ASSERT_TRUE(web->buttonBox->isEnabled());
    web->m_backButton->setEnabled(false);
    web->m_forwardButton->setEnabled(false);
    web->updateBtnBox();
    ASSERT_FALSE(web->buttonBox->isVisible());
    delete web;

}

TEST_F(ut_web_window_test, openjsPage)
{
    WebWindow *web = new WebWindow;
    Stub s;
    s.set((QWebEnginePage* (QWebEngineView::*)())ADDR(QWebEngineView, page), ADDR(ut_web_window_test, stub_page));
    s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel), ADDR(ut_web_window_test, stub_setWeb));
    s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QString&))ADDR(QWebEnginePage, runJavaScript), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QColor &))ADDR(QWebEnginePage, setBackgroundColor), ADDR(ut_web_window_test, stub_initweb));
    web->initWeb();
    web->bFinishChannel = true;
    web->openjsPage("deepin-terminal", "查找");

    web->openjsPage("", "查找");

    web->openjsPage("/deepin-terminal", "查找");
    delete webchannel;
    delete web;

}


TEST_F(ut_web_window_test, updatePage)
{
    WebWindow *web = new WebWindow;
    web->initUI();
    web->search_proxy_ = new SearchProxy;
    web->search_manager_ = new SearchManager;
    QStringList applistmd;
    applistmd.append("/usr/share/deepin-manual/manual-assets/application/dde-calendar/calendar/zh_HK/calendar.md");
    applistmd.append("/usr/share/deepin-manual/manual-assets/application/deepin-terminal/terminal/zh_CN/terminal.md");
    web->updatePage(applistmd);
    delete web;

}

bool ut_web_window_test::stub_isValid()
{
    return true;
}

TEST_F(ut_web_window_test, slot_HelpSupportTriggered)
{
    WebWindow *web = new WebWindow;
    web->HelpSupportTriggered();

    Stub st;
    st.set(ADDR(QDBusReply<void>, isValid), ADDR(ut_web_window_test, stub_isValid));
    web->HelpSupportTriggered(true);
    st.reset(ADDR(QDBusReply<void>, isValid));
    delete web;

}


TEST_F(ut_web_window_test, slotUpdateLabel)
{
    WebWindow *web = new WebWindow;
    web->slotUpdateLabel();
    delete web;
}

TEST_F(ut_web_window_test, closeEvent)
{
        WebWindow *web = new WebWindow;
        web->initUI();
        //web->initWeb();
        web->setFixedWidth(600);
        web->setFixedHeight(1200);
        web->close();
        QSettings *setting = ConfigManager::getInstance()->getSettings();
        setting->beginGroup(kConfigWindowInfo);
        ASSERT_EQ(setting->value(kConfigWindowWidth), 600);
        ASSERT_EQ(setting->value(kConfigWindowHeight), 1200);
        delete web;

}



TEST_F(ut_web_window_test, inputMethodQuery)
{
    WebWindow *web = new WebWindow;
    Qt::InputMethodQuery prop;
    prop = Qt::ImEnabled;
    ASSERT_TRUE(web->inputMethodQuery(prop).toBool());

    prop = Qt::ImCursorRectangle;
    web->inputMethodQuery(prop);

    delete web;

}

TEST_F(ut_web_window_test, eventFilter)
{
        WebWindow *web = new WebWindow;
        web->setObjectName("QMainWindowClassWindow");
        web->initUI();
        web->setFocus();
        qApp->setActiveWindow(web);
        //web->search_edit_->lineEdit()->setFocus();
        QTest::keyPress(web, Qt::Key_1);
        ASSERT_TRUE(web->search_edit_->lineEdit()->hasFocus());

        Stub s;
        s.set((QWebEnginePage* (QWebEngineView::*)())ADDR(QWebEngineView, page), ADDR(ut_web_window_test, stub_page));
        s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel), ADDR(ut_web_window_test, stub_setWeb));
        s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), ADDR(ut_web_window_test, stub_initweb));
        s.set((void (QWebEnginePage::*)(const QString&))ADDR(QWebEnginePage, runJavaScript), ADDR(ut_web_window_test, stub_initweb));
        s.set((QString (QWebEngineView::*)())ADDR(QWebEngineView, selectedText), ADDR(ut_web_window_test, stub_selectText));
        s.set((void (QWebEnginePage::*)(const QColor &))ADDR(QWebEnginePage, setBackgroundColor), ADDR(ut_web_window_test, stub_initweb));
        web->initWeb();



        QMouseEvent *evnReleaseEnter;
        evnReleaseEnter = new QMouseEvent( QEvent::MouseButtonRelease, QPoint(0, 0), Qt::RightButton, Qt::NoButton, Qt::NoModifier );
        web->eventFilter(web, evnReleaseEnter);
        delete evnReleaseEnter;


        evnReleaseEnter = new QMouseEvent( QEvent::MouseButtonPress, QPoint(0, 0), Qt::LeftButton, Qt::BackButton, Qt::NoModifier );
        s.set((Qt::MouseButton (QMouseEvent::*)())ADDR(QMouseEvent, button), ADDR(ut_web_window_test, stub_MouseButtonBack));
        web->eventFilter(web, evnReleaseEnter);
        s.reset((Qt::MouseButton (QMouseEvent::*)())ADDR(QMouseEvent, button));
        delete evnReleaseEnter;

        evnReleaseEnter = new QMouseEvent( QEvent::MouseButtonPress, QPoint(0, 0), Qt::LeftButton, Qt::BackButton, Qt::NoModifier );
        s.set((Qt::MouseButton (QMouseEvent::*)())ADDR(QMouseEvent, button), ADDR(ut_web_window_test, stub_MouseButtonForward));
        web->eventFilter(web, evnReleaseEnter);
        s.reset((Qt::MouseButton (QMouseEvent::*)())ADDR(QMouseEvent, button));
        delete evnReleaseEnter;

        evnReleaseEnter = new QMouseEvent( QEvent::MouseButtonPress, QPoint(0, 0), Qt::LeftButton, Qt::BackButton, Qt::NoModifier );
        s.set((Qt::MouseButton (QMouseEvent::*)())ADDR(QMouseEvent, button), ADDR(ut_web_window_test, stub_MouseButtonMiddle));
        web->eventFilter(web, evnReleaseEnter);
        s.reset((Qt::MouseButton (QMouseEvent::*)())ADDR(QMouseEvent, button));
        delete evnReleaseEnter;

        QEvent * ev = new QEvent(QEvent::FontChange);
        web->eventFilter(web, ev);
        delete ev;

        QApplication::clipboard()->setText("uos");
        QTest::keyPress(web, Qt::Key_V, Qt::ControlModifier);
        ASSERT_TRUE(web->search_edit_->lineEdit()->hasFocus());
        QTest::keyPress(web, Qt::Key_Enter);
        delete webchannel;
        delete web;

}

TEST_F(ut_web_window_test, onManualSearchByKeyword)
{


}

TEST_F(ut_web_window_test, onAppearanceChanged)
{
    QMap<QString, QVariant> map;
    QVariant var = "#00A48A";
    map.insert("QtActiveColor", var);
    WebWindow *web = new WebWindow;
    web->onAppearanceChanged("", map, QStringList());

    QMap<QString, QVariant> Iconvarmap;
    QVariant Iconvar = "#00A48A";
    Iconvarmap.insert("IconTheme", Iconvar);
    web->onAppearanceChanged("", Iconvarmap, QStringList());

    QVariant varStandard = "#00A48A";
    QMap<QString, QVariant> mapStandard;
    mapStandard.insert("StandardFont", varStandard);
    Stub s;
    s.set((QWebEnginePage* (QWebEngineView::*)())ADDR(QWebEngineView, page), ADDR(ut_web_window_test, stub_page));
    s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel), ADDR(ut_web_window_test, stub_setWeb));
    s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QString&))ADDR(QWebEnginePage, runJavaScript), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QColor &))ADDR(QWebEnginePage, setBackgroundColor), ADDR(ut_web_window_test, stub_initweb));
    web->initWeb();
    web->onAppearanceChanged("", mapStandard, QStringList());
    delete webchannel;
    delete web;

}


TEST_F(ut_web_window_test, saveWindowSize)
{
    WebWindow *web = new WebWindow;
    web->initUI();
    //web->initWeb();
    web->setFixedWidth(600);
    web->setFixedHeight(1200);
    web->saveWindowSize();
    QSettings *setting = ConfigManager::getInstance()->getSettings();

    setting->beginGroup(kConfigWindowInfo);
    ASSERT_EQ(setting->value(kConfigWindowWidth), 600);
    ASSERT_EQ(setting->value(kConfigWindowHeight), 1200);
    setting->endGroup();
    delete web;

}



TEST_F(ut_web_window_test, onSearchTextChanged)
{
        WebWindow *web = new WebWindow;
        web->initUI();
        web->onSearchTextChanged("应用");
        usleep(200000);
        ASSERT_FALSE(web->completion_window_->isVisible());
        delete web;

}


TEST_F(ut_web_window_test, onSearchTextChangedDelay)
{
        WebWindow *web = new WebWindow;
        web->initUI();
        web->search_edit_->setText("关闭应用商店");
        web->onSearchTextChangedDelay();
        ASSERT_EQ(web->completion_window_->keyword(), "关闭应用商店");
        delete web;

}
TEST_F(ut_web_window_test, onTitleBarEntered)
{
    WebWindow *web = new WebWindow;
    web->initUI();
    web->search_edit_->setText("关闭应用商店");
    web->onTitleBarEntered();
    ASSERT_FALSE(web->completion_window_->isVisible());
    delete web;

}

TEST_F(ut_web_window_test, onSetKeyword)
{
    WebWindow *web = new WebWindow;
    web->onSetKeyword("应用");
    ASSERT_EQ(web->search_edit_->text(), "应用");

    web->onSetKeyword("应用=-=");

    web->onSetKeyword("");
    delete web;

}
TEST_F(ut_web_window_test, onSearchAnchorResult)
{
    WebWindow *web = new WebWindow;
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
    ASSERT_FALSE(web->completion_window_->isVisible());
    web->onSearchAnchorResult("", list);
    ASSERT_TRUE(web->completion_window_->isVisible());
    ASSERT_EQ(web->completion_window_->result_view_->count(), 2);
    delete web;

}
TEST_F(ut_web_window_test, onSearchAnchorResult2)
{
    WebWindow *web = new WebWindow;
    SearchAnchorResultList list;
    SearchAnchorResult result;

    web->onSearchAnchorResult("", list);
    ASSERT_FALSE(web->completion_window_->isVisible());
    delete web;

}

TEST_F(ut_web_window_test, hasWidgetRect)
{
    WebWindow *web = new WebWindow;
    QWidget *wid = new QWidget;

    web->hasWidgetRect(wid);
    delete web;


}
QWebEnginePage* ut_web_window_test::stub_page()
{
    return (QWebEnginePage*)this;
}

QString ut_web_window_test::stub_selectText() const
{
    return "app";
}



void ut_web_window_test::stub_initweb()
{
    return;
}

Qt::MouseButton ut_web_window_test::stub_MouseButtonBack() const
{
    return Qt::BackButton;
}

Qt::MouseButton ut_web_window_test::stub_MouseButtonForward() const
{
    return Qt::ForwardButton;
}

Qt::MouseButton ut_web_window_test::stub_MouseButtonMiddle() const
{
    return Qt::MiddleButton;
}

DGuiApplicationHelper::ColorType ut_web_window_test::stub_themeType() const
{
    return DGuiApplicationHelper::LightType;
}

void ut_web_window_test::stub_setWeb(QWebChannel * m_webchannel)
{
    webchannel = m_webchannel;
    return;
}
TEST_F(ut_web_window_test, initWeb)
{
    WebWindow *web = new WebWindow;
    Stub s;
    s.set((QWebEnginePage* (QWebEngineView::*)())ADDR(QWebEngineView, page), ADDR(ut_web_window_test, stub_page));
    s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel), ADDR(ut_web_window_test, stub_setWeb));
    s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QString&))ADDR(QWebEnginePage, runJavaScript), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QColor &))ADDR(QWebEnginePage, setBackgroundColor), ADDR(ut_web_window_test, stub_initweb));
    web->initWeb();
    delete webchannel;
    delete web;

}
TEST_F(ut_web_window_test, inputMethodEvent)
{
    WebWindow *web = new WebWindow;
   QInputMethodEvent e;
   e.setCommitString("app");
   web->search_edit_->setText("app1");
    web->inputMethodEvent(&e);
    delete web;


}
TEST_F(ut_web_window_test, onWebPageLoadProgress)
{
    WebWindow *web = new WebWindow;
    Stub s;
    s.set((QWebEnginePage* (QWebEngineView::*)())ADDR(QWebEngineView, page), ADDR(ut_web_window_test, stub_page));
    s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel), ADDR(ut_web_window_test, stub_setWeb));
    s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QString&))ADDR(QWebEnginePage, runJavaScript), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QColor &))ADDR(QWebEnginePage, setBackgroundColor), ADDR(ut_web_window_test, stub_initweb));
    s.set((bool (Dtk::Widget::DSpinner::*)())ADDR(Dtk::Widget::DSpinner, isPlaying), ADDR(ut_web_window_test, stub_isValid));
    web->initWeb();
    web->onWebPageLoadProgress(30);
    delete webchannel;
    delete web;

}
TEST_F(ut_web_window_test, onSearchResultClicked)
{
    WebWindow *web = new WebWindow;
    Stub s;
    s.set((QWebEnginePage* (QWebEngineView::*)())ADDR(QWebEngineView, page), ADDR(ut_web_window_test, stub_page));
    s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel), ADDR(ut_web_window_test, stub_setWeb));
    s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QString&))ADDR(QWebEnginePage, runJavaScript), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QColor &))ADDR(QWebEnginePage, setBackgroundColor), ADDR(ut_web_window_test, stub_initweb));

    web->initWeb();
    SearchAnchorResult result;
    web->onSearchResultClicked(result);
    delete webchannel;
    delete web;
}
TEST_F(ut_web_window_test, onSearchButtonClicked)
{
    WebWindow *web = new WebWindow;
    Stub s;
    s.set((QWebEnginePage* (QWebEngineView::*)())ADDR(QWebEngineView, page), ADDR(ut_web_window_test, stub_page));
    s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel), ADDR(ut_web_window_test, stub_setWeb));
    s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QString&))ADDR(QWebEnginePage, runJavaScript), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QColor &))ADDR(QWebEnginePage, setBackgroundColor), ADDR(ut_web_window_test, stub_initweb));

    web->initWeb();
    web->onSearchButtonClicked();
    delete webchannel;
    delete web;

}
TEST_F(ut_web_window_test, settingContextMenu)
{
    WebWindow *web = new WebWindow;
    Stub s;
    s.set((QWebEnginePage* (QWebEngineView::*)())ADDR(QWebEngineView, page), ADDR(ut_web_window_test, stub_page));
    s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel), ADDR(ut_web_window_test, stub_setWeb));
    s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QString&))ADDR(QWebEnginePage, runJavaScript), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QColor &))ADDR(QWebEnginePage, setBackgroundColor), ADDR(ut_web_window_test, stub_initweb));

    web->initWeb();
    web->settingContextMenu();
    delete webchannel;
    delete web;
}

TEST_F(ut_web_window_test, initUI)
{
    WebWindow *web = new WebWindow;
    Stub s;
    s.set(ADDR(Utils, hasSelperSupport), ADDR(ut_web_window_test, stub_isValid));
    web->initUI();
    delete web;
}


TEST_F(ut_web_window_test, setHashWordColor)
{
    WebWindow *web = new WebWindow;
    Stub s;
    s.set((QWebEnginePage* (QWebEngineView::*)())ADDR(QWebEngineView, page), ADDR(ut_web_window_test, stub_page));
    s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel), ADDR(ut_web_window_test, stub_setWeb));
    s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QString&))ADDR(QWebEnginePage, runJavaScript), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QColor &))ADDR(QWebEnginePage, setBackgroundColor), ADDR(ut_web_window_test, stub_initweb));

    web->initWeb();
    web->setHashWordColor();
    delete webchannel;
    delete web;
    //webchannel= nullptr;
}

TEST_F(ut_web_window_test, onChannelFinish)
{
    WebWindow *web = new WebWindow;
    Stub s;
    s.set((QWebEnginePage* (QWebEngineView::*)())ADDR(QWebEngineView, page), ADDR(ut_web_window_test, stub_page));
    s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel), ADDR(ut_web_window_test, stub_setWeb));
    s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QString&))ADDR(QWebEnginePage, runJavaScript), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QColor &))ADDR(QWebEnginePage, setBackgroundColor), ADDR(ut_web_window_test, stub_initweb));

    web->initWeb();
    web->app_name_ = "";
    web->onChannelFinish();

    web->app_name_ = "app";
    web->onChannelFinish();

    s.set((DGuiApplicationHelper::ColorType (DGuiApplicationHelper::*)() const)ADDR(DGuiApplicationHelper, themeType), ADDR(ut_web_window_test, stub_themeType));
    web->app_name_ = "/app";
    web->onChannelFinish();

    web->first_webpage_loaded_ = true;
    web->keyword_ = "11";
    web->onChannelFinish();
    delete webchannel;
    delete web;
}


TEST_F(ut_web_window_test, slot_ThemeChanged)
{
    WebWindow *web = new WebWindow;
    Stub s;
    s.set((QWebEnginePage* (QWebEngineView::*)())ADDR(QWebEngineView, page), ADDR(ut_web_window_test, stub_page));
    s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel), ADDR(ut_web_window_test, stub_setWeb));
    s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QString&))ADDR(QWebEnginePage, runJavaScript), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QColor &))ADDR(QWebEnginePage, setBackgroundColor), ADDR(ut_web_window_test, stub_initweb));
    s.set((DGuiApplicationHelper::ColorType (DGuiApplicationHelper::*)() const)ADDR(DGuiApplicationHelper, themeType), ADDR(ut_web_window_test, stub_themeType));

    web->initWeb();
    web->slot_ThemeChanged();
    delete webchannel;
    delete web;
}

TEST_F(ut_web_window_test, cancelTextChanged)
{
    WebWindow *web = new WebWindow;
    Stub s;
    s.set((QWebEnginePage* (QWebEngineView::*)())ADDR(QWebEngineView, page), ADDR(ut_web_window_test, stub_page));
    s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel), ADDR(ut_web_window_test, stub_setWeb));
    s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QString&))ADDR(QWebEnginePage, runJavaScript), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QColor &))ADDR(QWebEnginePage, setBackgroundColor), ADDR(ut_web_window_test, stub_initweb));
    web->initWeb();
    web->cancelTextChanged();
    delete webchannel;
    delete  web;
}


TEST_F(ut_web_window_test, setAppProperty)
{
    WebWindow *web = new WebWindow;
    web->setAppProperty("", "", "");
    delete  web;
}

TEST_F(ut_web_window_test, resizeEvent)
{
    WebWindow *web = new WebWindow;

    web->resizeEvent(nullptr);
    delete  web;
}

TEST_F(ut_web_window_test, initConnections)
{
    WebWindow *web = new WebWindow;

    web->initConnections();
    delete  web;
}

TEST_F(ut_web_window_test, onSearchEditFocusOut)
{
    WebWindow *web = new WebWindow;

    web->onSearchEditFocusOut();
    delete  web;
}

