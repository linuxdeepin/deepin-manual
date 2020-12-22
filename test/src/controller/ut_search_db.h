#ifndef UT_SEARCH_DB_TEST_H
#define UT_SEARCH_DB_TEST_H

#include "gtest/gtest.h"
#include <QtTest>

class SearchDb;
class ut_search_db_test : public ::testing::Test
{
public:
    ut_search_db_test();
    virtual void SetUp() override;
    virtual void TearDown() override;
    SearchDb *sd;
};

#endif // UT_SEARCH_DB_TEST_H
