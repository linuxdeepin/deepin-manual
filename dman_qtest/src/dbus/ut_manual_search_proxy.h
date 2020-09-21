#ifndef UT_MANUAL_SEARCH_PROXY_H
#define UT_MANUAL_SEARCH_PROXY_H

#include "gtest/gtest.h"
#include "QtTest"

class ManualSearchProxy;

class ut_manual_search_proxy : public::testing::Test
{

public:
    ut_manual_search_proxy();
    virtual void SetUp() override;
    virtual void TearDown() override;
    ManualSearchProxy *msp;

};

#endif // UT_MANUAL_SEARCH_PROXY_H
