// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UT_CONFIG_MANAGER_TEST_H
#define UT_CONFIG_MANAGER_TEST_H

#include "gtest/gtest.h"
#include <QTest>

class ConfigManager;

class ut_config_manager_test : public ::testing::Test
{
public:
    ut_config_manager_test();

protected:
    ConfigManager *m_ap = nullptr;
};

#endif // UT_CONFIG_MANAGER_TEST_H
