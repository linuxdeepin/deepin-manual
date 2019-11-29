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

#include "view/title_bar_proxy.h"

namespace dman {

TitleBarProxy::TitleBarProxy(QObject *parent)
    : QObject(parent)
{
    m_webWindow = static_cast<WebWindow *>(parent);
    m_first = 0;
}

TitleBarProxy::~TitleBarProxy()
{
}

void TitleBarProxy::setBackwardButtonActive(bool active)
{
    m_first++;
    if(m_first == 3)
        emit buttonShowSignal();

    m_webWindow->m_backButton->setEnabled(active);
}

void TitleBarProxy::setForwardButtonActive(bool active)
{
    m_webWindow->m_forwardButton->setEnabled(active);
}

}  // namespace dman
