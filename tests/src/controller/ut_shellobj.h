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
#ifndef UT_SHELLOBJ_H
#define UT_SHELLOBJ_H

#include "gtest/gtest.h"

#include <QObject>
#include <QtTest>

class shellObj;
class ut_shellObj : public::testing::Test
{
public:
    ut_shellObj();
    virtual void SetUp() override;
    virtual void TearDown() override;
protected:
    shellObj *m_so = nullptr;
};

#endif // UT_SHELLOBJ_H
