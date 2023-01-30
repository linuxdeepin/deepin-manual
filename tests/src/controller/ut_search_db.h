// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UT_SEARCH_DB_TEST_H
#define UT_SEARCH_DB_TEST_H

#include "gtest/gtest.h"
#include "src/third-party/stub/stub.h"

#include <QtTest>

class SearchDb;
class ut_search_db_test : public ::testing::Test
{
public:
    ut_search_db_test();
    virtual void SetUp() override;
    virtual void TearDown() override;

    SearchDb *sd = nullptr;
    static Stub *stt;

    QVariant stub_value(int num) const;
    QVariant stub_valueh3(int num) const;
    QVariant stub_nextfalse(int num) const;
    bool stub_transactionture() const;
    QVariant stub_valuenoh0(int num) const;

    QString stub_writableLocation(QStandardPaths::StandardLocation type);
};

#endif // UT_SEARCH_DB_TEST_H
