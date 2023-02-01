// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UT_MANUAL_SEARCH_PROXY_H
#define UT_MANUAL_SEARCH_PROXY_H

#include "gtest/gtest.h"
#include "QtTest"

class ManualSearchProxy;

class ut_manual_search_proxy : public ::testing::Test
{
public:
    ut_manual_search_proxy();
    virtual void SetUp() override;
    virtual void TearDown() override;
    ManualSearchProxy *msp = nullptr;
};

#endif // UT_MANUAL_SEARCH_PROXY_H
