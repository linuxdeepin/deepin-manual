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
#include "ut_shellobj.h"
#include "controller/shellobj.h"

ut_shellObj::ut_shellObj()
{

}

void ut_shellObj::SetUp()
{
    m_so = new shellObj();
}

void ut_shellObj::TearDown()
{
    delete m_so;
}

TEST_F(ut_shellObj, execSystem)
{
    shellObj &sh = m_so->execSystem("ls");
}
