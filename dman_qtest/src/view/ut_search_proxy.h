#ifndef UT_SEARCH_PROXY_TEST_H
#define UT_SEARCH_PROXY_TEST_H

#include "gtest/gtest.h"
#include <QTest>

namespace dman {

class SearchProxy;
class ut_search_proxy_test : public  testing::Test
{
public:
    ut_search_proxy_test();
    virtual void SetUp() override;
    virtual void TearDown() override;

public:
    SearchProxy *m_searchProxy;
};
}
#endif // UT_SEARCH_PROXY_TEST_H
