/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     wangmingliang <wangmingliang@uniontech.com>
* Maintainer: wangmingliang <wangmingliang@uniontech.com>
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
#include "ut_titlebarproxy.h"
#include "view/title_bar_proxy.h"
#include "view/widget/search_edit.h"
#include "view/widget/search_completion_window.h"
#include "view/widget/search_button.h"
#include "controller/search_manager.h"
#include "controller/search_result.h"
#include "base/consts.h"
#include "controller/config_manager.h"

ut_TitleBarProxy::ut_TitleBarProxy()
{

}

void ut_TitleBarProxy::SetUp()
{

    web.initUI();
    m_tbp = new TitleBarProxy(&web);
}

void ut_TitleBarProxy::TearDown()
{
    delete m_tbp;
}

TEST_F(ut_TitleBarProxy, setBackwardButtonActive)
{
    web.web_view_ = new QWebEngineView;
    m_tbp->setBackwardButtonActive(true);
    ASSERT_TRUE(m_tbp->m_webWindow->m_backButton->isEnabled());
    delete web.web_view_ ;
}

TEST_F(ut_TitleBarProxy, setForwardButtonActive)
{
    m_tbp->setForwardButtonActive(true);
    ASSERT_TRUE(m_tbp->m_webWindow->m_forwardButton->isEnabled());
}
