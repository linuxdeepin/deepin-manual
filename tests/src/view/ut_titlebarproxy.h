// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UT_TITLEBARPROXY_H
#define UT_TITLEBARPROXY_H

#include "view/web_window.h"
#include "gtest/gtest.h"

#include <QTest>

class TitleBarProxy;

class ut_TitleBarProxy : public testing::Test
{
public:
    ut_TitleBarProxy();

    virtual void SetUp() override;
    virtual void TearDown() override;
    TitleBarProxy *m_tbp = nullptr;
     WebWindow web;

};

#endif // UT_TITLEBARPROXY_H
