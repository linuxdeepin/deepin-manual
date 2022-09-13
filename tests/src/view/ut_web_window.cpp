// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_web_window.h"
#include "view/web_window.h"
#include "view/widget/search_edit.h"
#include "view/widget/search_completion_window.h"
#include "view/widget/search_button.h"
#include "view/widget/image_viewer.h"
#include "view/i18n_proxy.h"
#include "view/image_viewer_proxy.h"
#include "view/manual_proxy.h"
#include "view/search_proxy.h"
#include "view/settings_proxy.h"
#include "view/theme_proxy.h"
#include "view/title_bar_proxy.h"
#include "controller/search_manager.h"
#include "controller/search_result.h"
#include "controller/config_manager.h"
#include "base/consts.h"
#include "base/utils.h"
#include "src/third-party/stub/stub.h"

#include <DTitlebar>

#include <QClipboard>
#include <QSignalSpy>

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
    Stub s;
    s.set((QWebEnginePage * (QWebEngineView::*)()) ADDR(QWebEngineView, page), ADDR(ut_web_window_test, stub_page));
    s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel), ADDR(ut_web_window_test, stub_setWeb));
    s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QString &))ADDR(QWebEnginePage, runJavaScript), ADDR(ut_web_window_test, stub_initweb));
    s.set((QString(QWebEngineView::*)())ADDR(QWebEngineView, selectedText), ADDR(ut_web_window_test, stub_selectText));
    s.set((void (QWebEnginePage::*)(const QColor &))ADDR(QWebEnginePage, setBackgroundColor), ADDR(ut_web_window_test, stub_initweb));
    web->initUI();
    web->initWeb();

    QSignalSpy spy(web->search_proxy_, SIGNAL(reloadPage(const QStringList &)));
    QStringList applistmd;
    applistmd.append("/usr/share/deepin-manual/manual-assets/application/dde-calendar/calendar/zh_HK/calendar.md");
    applistmd.append("/usr/share/deepin-manual/manual-assets/application/deepin-terminal/terminal/zh_CN/terminal.md");
    web->updatePage(applistmd);
    ASSERT_EQ(spy.count(), 1);

    delete webchannel;
    delete web;

}

bool ut_web_window_test::stub_isValid()
{
    return true;
}

bool ut_web_window_test::stub_isValidfalse()
{
    return false;
}

bool ut_HelpSupportTriggered = false;

QDBusMessage ut_HelpSupport(QDBus::CallMode mode,
                            const QString &method,
                            const QVariant &arg1 = QVariant(),
                            const QVariant &arg2 = QVariant(),
                            const QVariant &arg3 = QVariant(),
                            const QVariant &arg4 = QVariant(),
                            const QVariant &arg5 = QVariant(),
                            const QVariant &arg6 = QVariant(),
                            const QVariant &arg7 = QVariant(),
                            const QVariant &arg8 = QVariant())
{
    ut_HelpSupportTriggered = true;
    QDBusMessage busmsg;
    return busmsg;
}

TEST_F(ut_web_window_test, slot_HelpSupportTriggered)
{
    WebWindow *web = new WebWindow;
    web->HelpSupportTriggered();

    Stub st;
    st.set(ADDR(QDBusReply<void>, isValid), ADDR(ut_web_window_test, stub_isValid));
    st.set((QDBusMessage(QDBusAbstractInterface::*)(QDBus::CallMode, const QString &,
                                                    const QVariant &, const QVariant &, const QVariant &, const QVariant &,
                                                    const QVariant &, const QVariant &, const QVariant &, const QVariant &))ADDR(QDBusAbstractInterface, call),
           ut_HelpSupport);

    web->HelpSupportTriggered(true);
    ASSERT_TRUE(ut_HelpSupportTriggered);
    st.reset(ADDR(QDBusReply<void>, isValid));
    st.set(ADDR(QDBusReply<void>, isValid), ADDR(ut_web_window_test, stub_isValidfalse));
    web->HelpSupportTriggered(true);
    ASSERT_TRUE(ut_HelpSupportTriggered);
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
    ASSERT_FALSE(web->inputMethodQuery(prop).toBool());
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
    s.set((QWebEnginePage * (QWebEngineView::*)()) ADDR(QWebEngineView, page), ADDR(ut_web_window_test, stub_page));
    s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel), ADDR(ut_web_window_test, stub_setWeb));
    s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QString &))ADDR(QWebEnginePage, runJavaScript), ADDR(ut_web_window_test, stub_initweb));
    s.set((QString(QWebEngineView::*)())ADDR(QWebEngineView, selectedText), ADDR(ut_web_window_test, stub_selectText));
    s.set((void (QWebEnginePage::*)(const QColor &))ADDR(QWebEnginePage, setBackgroundColor), ADDR(ut_web_window_test, stub_initweb));
    web->initWeb();

    QMouseEvent *evnReleaseEnter;
    evnReleaseEnter = new QMouseEvent(QEvent::MouseButtonRelease, QPoint(0, 0), Qt::RightButton, Qt::NoButton, Qt::NoModifier);
    web->eventFilter(web, evnReleaseEnter);
    delete evnReleaseEnter;

    QSignalSpy spy(web->title_bar_proxy_, SIGNAL(backwardButtonClicked()));
    evnReleaseEnter = new QMouseEvent(QEvent::MouseButtonPress, QPoint(0, 0), Qt::LeftButton, Qt::BackButton, Qt::NoModifier);
    s.set((Qt::MouseButton(QMouseEvent::*)())ADDR(QMouseEvent, button), ADDR(ut_web_window_test, stub_MouseButtonBack));
    web->eventFilter(web, evnReleaseEnter);
    s.reset((Qt::MouseButton(QMouseEvent::*)())ADDR(QMouseEvent, button));
    delete evnReleaseEnter;
    //ASSERT_EQ(spy.count(), 1);

    QSignalSpy spy1(web->title_bar_proxy_, SIGNAL(forwardButtonClicked()));
    evnReleaseEnter = new QMouseEvent(QEvent::MouseButtonPress, QPoint(0, 0), Qt::LeftButton, Qt::BackButton, Qt::NoModifier);
    s.set((Qt::MouseButton(QMouseEvent::*)())ADDR(QMouseEvent, button), ADDR(ut_web_window_test, stub_MouseButtonForward));
    web->eventFilter(web, evnReleaseEnter);
    s.reset((Qt::MouseButton(QMouseEvent::*)())ADDR(QMouseEvent, button));
    delete evnReleaseEnter;
    //         ASSERT_EQ(spy1.count(), 1);

    evnReleaseEnter = new QMouseEvent(QEvent::MouseButtonPress, QPoint(0, 0), Qt::LeftButton, Qt::BackButton, Qt::NoModifier);
    s.set((Qt::MouseButton(QMouseEvent::*)())ADDR(QMouseEvent, button), ADDR(ut_web_window_test, stub_MouseButtonMiddle));
    web->eventFilter(web, evnReleaseEnter);
    s.reset((Qt::MouseButton(QMouseEvent::*)())ADDR(QMouseEvent, button));
    delete evnReleaseEnter;

    QEvent *ev = new QEvent(QEvent::FontChange);
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
    Stub s;
    s.set((QWebEnginePage * (QWebEngineView::*)()) ADDR(QWebEngineView, page), ADDR(ut_web_window_test, stub_page));
    s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel), ADDR(ut_web_window_test, stub_setWeb));
    s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QString &))ADDR(QWebEnginePage, runJavaScript), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QColor &))ADDR(QWebEnginePage, setBackgroundColor), ADDR(ut_web_window_test, stub_initweb));
    web->initWeb();
    QSignalSpy spy(web->manual_proxy_, SIGNAL(iconThemeChanged(const QString &)));
    web->onAppearanceChanged("", map, QStringList());

    QMap<QString, QVariant> Iconvarmap;
    QVariant Iconvar = "#00A48A";
    Iconvarmap.insert("IconTheme", Iconvar);
    web->onAppearanceChanged("", Iconvarmap, QStringList());
    ASSERT_EQ(spy.count(), 1);

    QVariant varStandard = "#00A48A";
    QMap<QString, QVariant> mapStandard;
    mapStandard.insert("StandardFont", varStandard);

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
    ASSERT_EQ(web->search_edit_->text(), "应用%");
    web->onSetKeyword("");
    ASSERT_TRUE(web->search_edit_->text().isEmpty());
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
    wid->setFixedSize(200, 200);

    QRect rect = web->hasWidgetRect(wid);
    ASSERT_EQ(rect.width(), wid->width());
    ASSERT_EQ(rect.height(), wid->height());
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

DGuiApplicationHelper::ColorType ut_web_window_test::stub_themeTypeDark() const
{
    return DGuiApplicationHelper::DarkType;
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
    QSettings *setting = ConfigManager::getInstance()->getSettings();
    setting->beginGroup(kConfigAppList);
    ASSERT_FALSE(setting->value("dde").toBool());
    setting->endGroup();
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
    ASSERT_TRUE(web->search_edit_->text().compare("app") == 0);
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
    ASSERT_FALSE(web->m_spinner->isVisible());
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
    ASSERT_TRUE(web->completion_window_->isHidden());
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
    ASSERT_EQ(web->web_view_->contextMenuPolicy(), Qt::CustomContextMenu);
    delete webchannel;
    delete web;
}

TEST_F(ut_web_window_test, initUI)
{
    WebWindow *web = new WebWindow;
    Stub s;
    s.set(ADDR(Utils, hasSelperSupport), ADDR(ut_web_window_test, stub_isValid));
    web->initUI();
    ASSERT_EQ(web->m_spinner->width(), 50);
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
    QColor Color = DGuiApplicationHelper::instance()->applicationPalette().highlight().color();
    ASSERT_EQ(web->completion_window_->search_button_->myColor, Color);
    delete webchannel;
    delete web;
    //webchannel= nullptr;
}

TEST_F(ut_web_window_test, onChannelFinish)
{
    WebWindow *web = new WebWindow;
    QSignalSpy spy(web, SIGNAL(manualSearchByKeyword(const QString &)));
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
    ASSERT_EQ(spy.count(), 1);
    delete webchannel;
    delete web;
}


TEST_F(ut_web_window_test, slot_ThemeChanged_001)
{
    Stub s;
    s.set(ADDR(Utils, judgeWayLand), ADDR(ut_web_window_test, stub_isValidfalse));
    WebWindow *web = new WebWindow;
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

TEST_F(ut_web_window_test, slot_ThemeChanged_002)
{
    Stub s;
    s.set(ADDR(Utils, judgeWayLand), ADDR(ut_web_window_test, stub_isValid));
    WebWindow *web = new WebWindow;
    s.set((QWebEnginePage* (QWebEngineView::*)())ADDR(QWebEngineView, page), ADDR(ut_web_window_test, stub_page));
    s.set((void (QWebEnginePage::*)(QWebChannel *))ADDR(QWebEnginePage, setWebChannel), ADDR(ut_web_window_test, stub_setWeb));
    s.set((void (QWebEngineView::*)(const QUrl &))ADDR(QWebEngineView, load), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QString&))ADDR(QWebEnginePage, runJavaScript), ADDR(ut_web_window_test, stub_initweb));
    s.set((void (QWebEnginePage::*)(const QColor &))ADDR(QWebEnginePage, setBackgroundColor), ADDR(ut_web_window_test, stub_initweb));
    s.set((DGuiApplicationHelper::ColorType (DGuiApplicationHelper::*)() const)ADDR(DGuiApplicationHelper, themeType), ADDR(ut_web_window_test, stub_themeType));

    web->initWeb();
    web->slot_ThemeChanged();

    DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
    if (themeType == DGuiApplicationHelper::DarkType) {
        QPalette pa = web->palette();
        QColor clor = pa.color(QPalette::Window);
        EXPECT_EQ(clor, QColor("#161616"));
    }else {
        QPalette pa = web->palette();
        QColor clor = pa.color(QPalette::Window);
        EXPECT_EQ(clor, Qt::white);
    }

    s.set((DGuiApplicationHelper::ColorType (DGuiApplicationHelper::*)() const)ADDR(DGuiApplicationHelper, themeType), ADDR(ut_web_window_test, stub_themeTypeDark));
    web->slot_ThemeChanged();

    themeType = DGuiApplicationHelper::instance()->themeType();
    if (themeType == DGuiApplicationHelper::DarkType) {
        QPalette pa = web->palette();
        QColor clor = pa.color(QPalette::Window);
        EXPECT_EQ(clor, QColor("#161616"));
    }else {
        QPalette pa = web->palette();
        QColor clor = pa.color(QPalette::Window);
        EXPECT_EQ(clor, Qt::white);
    }

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
    ASSERT_EQ(web->web_view_->contextMenuPolicy(), Qt::NoContextMenu);
    delete webchannel;
    delete  web;
}


TEST_F(ut_web_window_test, setAppProperty)
{
    WebWindow *web = new WebWindow;
    web->setAppProperty("1", "2", "3");
    ASSERT_EQ(web->keyword_, "3");
    ASSERT_EQ(web->app_name_, "1");
    ASSERT_EQ(web->title_name_, "2");
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
    ASSERT_TRUE(web->completion_window_->isHidden());
    delete  web;
}


TEST_F(ut_web_window_test, resizeEvent_001)
{
    Stub s;
    s.set(ADDR(QWidget, isVisible), ADDR(ut_web_window_test, stub_isValid));
    s.set(ADDR(Utils, judgeWayLand), ADDR(ut_web_window_test, stub_isValid));
    WebWindow *web = new WebWindow;
    SearchAnchorResult result2;
    SearchAnchorResultList list;
    result2.anchor = "运行应用商店";
    result2.anchorId = "h3";
    result2.app_name = "deepin-app-store";
    result2.app_display_name = "应用商店";
    list.append(result2);
    web->onSearchAnchorResult("", list);

    web->completion_window_->setVisible(true);
    web->resize(web->size()+QSize(1,1));

    const QPoint local_point(web->rect().width() / 2 - web->search_edit_->width() / 2,
                             web->titlebar()->height() - 3);

    EXPECT_EQ(local_point, web->completion_window_->pos());

    delete  web;
}


TEST_F(ut_web_window_test, resizeEvent_002)
{
    Stub s;
    s.set(ADDR(QWidget, isVisible), ADDR(ut_web_window_test, stub_isValid));
    s.set(ADDR(Utils, judgeWayLand), ADDR(ut_web_window_test, stub_isValidfalse));
    WebWindow *web = new WebWindow;
    SearchAnchorResult result2;
    SearchAnchorResultList list;
    result2.anchor = "运行应用商店";
    result2.anchorId = "h3";
    result2.app_name = "deepin-app-store";
    result2.app_display_name = "应用商店";
    list.append(result2);
    web->onSearchAnchorResult("", list);

    web->completion_window_->setVisible(true);
    web->resize(web->size()+QSize(1,1));

    const QPoint local_point(web->rect().width() / 2 - web->search_edit_->width() / 2,
                             web->titlebar()->height() - 3);

    EXPECT_EQ(local_point, web->completion_window_->pos());

    delete  web;
}

