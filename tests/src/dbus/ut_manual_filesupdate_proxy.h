// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UT_MANUAL_FILESUPDATE_PROXY_H
#define UT_MANUAL_FILESUPDATE_PROXY_H

#include "gtest/gtest.h"
#include <QTest>

class ManualFilesUpdateProxy;
class ut_ManualFilesUpdateProxy : public testing::Test
{
public:
    ut_ManualFilesUpdateProxy();
    virtual void SetUp() override;
    virtual void TearDown() override;
    ManualFilesUpdateProxy *mfuProxy = nullptr;
};

#endif // UT_MANUAL_FILESUPDATE_PROXY_H
