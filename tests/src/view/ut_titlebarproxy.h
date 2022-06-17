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
#ifndef UT_TITLEBARPROXY_H
#define UT_TITLEBARPROXY_H

#include "view/web_window.h"
#include "gtest/gtest.h"

#include <QTest>

class TitleBarProxy;

class ut_TitleBarProxy : public testing::Test
{
public:
    ut_TitleBarProxy();

    virtual void SetUp() override;
    virtual void TearDown() override;
    TitleBarProxy *m_tbp = nullptr;
     WebWindow web;

};

#endif // UT_TITLEBARPROXY_H
