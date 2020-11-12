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

#ifndef DEEPIN_MANUAL_VIEWS_TITLE_BAR_PROXY_H
#define DEEPIN_MANUAL_VIEWS_TITLE_BAR_PROXY_H

#include "web_window.h"

#include <QObject>

/**
 * @brief The TitleBarProxy class
 * 前进后退按钮控制接口类，ｊｓ调用
 */
class TitleBarProxy : public QObject
{
    Q_OBJECT

public:
    TitleBarProxy(/*TitleBar* title_bar,*/ QObject *parent = nullptr);
    ~TitleBarProxy() override;

signals:
    void backwardButtonClicked();
    void forwardButtonClicked();

public slots:
    void setBackwardButtonActive(bool active);
    void setForwardButtonActive(bool active);

private:
    WebWindow *m_webWindow;
    int m_first;
};

#endif // DEEPIN_MANUAL_VIEWS_TITLE_BAR_PROXY_H
