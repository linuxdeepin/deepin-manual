// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
