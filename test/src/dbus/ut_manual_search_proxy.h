/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     wangmingliang <wangmingliang@uniontech.com>
* Maintainer: wangmingliang <wangmingliang@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
    ManualSearchProxy *msp;
};

#endif // UT_MANUAL_SEARCH_PROXY_H
