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

#ifndef UT_SEARCH_MANAGER_TEST_H
#define UT_SEARCH_MANAGER_TEST_H

#include "gtest/gtest.h"
#include <QTest>


class ut_search_manager_test : public ::testing::Test
{
public:
    ut_search_manager_test();
    QString stub_writableLocation();
};

#endif // UT_SEARCH_MANAGER_TEST_H
