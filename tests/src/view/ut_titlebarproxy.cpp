// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
