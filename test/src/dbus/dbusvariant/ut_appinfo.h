#ifndef UT_APPINFO_H
#define UT_APPINFO_H

#include "gtest/gtest.h"
#include <QTest>

class AppInfo;
class ut_AppInfo :public testing::Test
{
public:
    ut_AppInfo();
    virtual void SetUp() override;
    virtual void TearDown() override;
    AppInfo *app;

};

#endif // UT_APPINFO_H
