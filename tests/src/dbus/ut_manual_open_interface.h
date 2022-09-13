// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UT_MANUAL_OPEN_INTERFACE_H
#define UT_MANUAL_OPEN_INTERFACE_H

#include "gtest/gtest.h"
#include <QTest>

class ManualOpenInterface;
class ut_ManualOpenInterface : public testing::Test
{
public:
    ut_ManualOpenInterface();
    virtual void SetUp() override;
    virtual void TearDown() override;
    ManualOpenInterface *interface = nullptr;
};

#endif // UT_MANUALOPENINTERFACE_H
