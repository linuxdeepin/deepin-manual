#ifndef UT_MANUAL_OPEN_PROXY_H
#define UT_MANUAL_OPEN_PROXY_H

#include "gtest/gtest.h"
#include <QTest>

class ManualOpenProxy;

class ut_manual_open_proxy : public::testing::Test
{
public:
    ut_manual_open_proxy();
    virtual void SetUp() override;
    virtual void TearDown() override;

protected:
    ManualOpenProxy *mopen;

};

#endif // UT_MANUAL_OPEN_PROXY_H
