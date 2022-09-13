// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UT_SEARCHEDIT_TEST_H
#define UT_SEARCHEDIT_TEST_H

#include "view/widget/search_edit.h"
#include "gtest/gtest.h"
#include <QTest>
#include <QObject>


class ut_SearchEdit_test: public::testing::Test
{
//    Q_OBJECT
public:
    ut_SearchEdit_test();

protected:
    SearchEdit *m_se = nullptr;
};

#endif // UT_SEARCHEDIT_TEST_H
