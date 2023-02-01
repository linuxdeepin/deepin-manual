// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UT_JSCONTEXT_H
#define UT_JSCONTEXT_H

#include "gtest/gtest.h"
#include <QTest>

class JsContext;

class ut_JsContext : public testing::Test
{
public:
    ut_JsContext();
    virtual void SetUp() override;
    virtual void TearDown() override;
    JsContext *m_jc = nullptr;
};

#endif // UT_JSCONTEXT_H
