// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "view/title_bar_proxy.h"

TitleBarProxy::TitleBarProxy(QObject *parent)
    : QObject(parent)
{
    m_webWindow = static_cast<WebWindow *>(parent);
    m_first = 0;
}

TitleBarProxy::~TitleBarProxy()
{
}

/**
 * @brief TitleBarProxy::setBackwardButtonActive
 * @param active true为显示
 * @note 控制后退按钮激活状态
 */
void TitleBarProxy::setBackwardButtonActive(bool active)
{
    m_webWindow->cancelTextChanged();
    m_webWindow->m_backButton->setEnabled(active);
    m_webWindow->updateBtnBox();
}

/**
 * @brief TitleBarProxy::setForwardButtonActive
 * @param active
 * @note 控制前进按钮激活状态
 */
void TitleBarProxy::setForwardButtonActive(bool active)
{
    m_webWindow->m_forwardButton->setEnabled(active);
    m_webWindow->updateBtnBox();
}
