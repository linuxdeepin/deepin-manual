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
#include "ut_searchedit_test.h"

namespace dman {

ut_SearchEdit_test::ut_SearchEdit_test()
{

}

void ut_SearchEdit_test::SetUp()
{
    if (m_se == nullptr)
        qDebug() << "null";
    else {
        qDebug() << "!= null";
    }

    m_se = new SearchEdit;


//    m_se->show();
}

void ut_SearchEdit_test::TearDown()
{
    delete m_se;
}

TEST_F(ut_SearchEdit_test, getStr)
{
    ASSERT_EQ(m_se->getStr(), "abc");
}
//TEST_F(ut_SearchEdit_test, getStr1)
//{
//    ASSERT_EQ(m_se->getStr(), "");
//}
}
