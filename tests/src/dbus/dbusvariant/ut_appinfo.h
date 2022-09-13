// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UT_APPINFO_H
#define UT_APPINFO_H

#include "gtest/gtest.h"
#include <QTest>

class AppInfo;
class ut_AppInfo : public testing::Test
{
public:
    ut_AppInfo();
    virtual void SetUp() override;
    virtual void TearDown() override;
    AppInfo *app = nullptr;
};

#endif // UT_APPINFO_H
