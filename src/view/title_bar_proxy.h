// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
    TitleBarProxy(QObject *parent = nullptr);
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
