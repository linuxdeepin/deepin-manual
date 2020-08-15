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
#include "ut_web_window_test.h"

#define private public
#include "view/web_window.h"
#include "view/widget/search_edit.h"
#include "view/widget/search_completion_window.h"
#include "view/widget/search_button.h"
#include "controller/search_manager.h"
#include "controller/search_result.h"

#undef private
namespace dman {

ut_web_window_test::ut_web_window_test()
{

}

void ut_web_window_test::SetUp()
{
//    m_webwindow = new WebWindow;
//    m_webwindow->show();
}

void ut_web_window_test::TearDown()
{
//    delete m_webwindow;
}
TEST_F(ut_web_window_test, UI)
{
//    m_webwindow->show();
    WebWindow web;
    //web.initUI();
    //web.initConnections();
//    web.initWeb();
    web.search_edit_->setText("应用");
    web.onChannelFinish();// resultClicked
    //QTest::keyClick(web.search_edit_, Qt::Key_Enter);
    //QString str = web.completion_window_->search_button_->m_textLabel->text();
    qDebug() << "ui---> " << web.keyword_;
    ASSERT_EQ(web.keyword_, "应用");
//    m_webwindow->show();
}

}
