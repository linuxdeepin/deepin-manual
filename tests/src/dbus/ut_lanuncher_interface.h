// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UT_LANUNCHER_INTERFACE_TEST_H
#define UT_LANUNCHER_INTERFACE_TEST_H

#include "gtest/gtest.h"
#include <QTest>

class LauncherInterface;

class ut_lanuncher_interface_test : public ::testing::Test
{
public:
    ut_lanuncher_interface_test();
    virtual void SetUp() override;
    virtual void TearDown() override;
    LauncherInterface *li = nullptr;

private:
};

#endif // UT_LANUNCHER_INTERFACE_TEST_H
