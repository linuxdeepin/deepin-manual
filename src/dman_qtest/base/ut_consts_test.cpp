/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     wangmingliang <wangmingliang@uniontech.com>
* Maintainer: wangmingliang <wanmgmingliang@uniontech.com>
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
#include "ut_consts_test.h"

namespace dman {


ut_consts_test::ut_consts_test()
{

}

void ut_consts_test::SetUp()
{
}

void ut_consts_test::TearDown()
{
}

TEST_F(ut_consts_test, GetCacheDir)
{
    ASSERT_EQ(GetCacheDir(), QString("/home/kevin_w/.cache/deepin/deepin-manual"));
}

}
