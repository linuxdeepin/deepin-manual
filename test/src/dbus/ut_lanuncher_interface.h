#ifndef UT_LANUNCHER_INTERFACE_TEST_H
#define UT_LANUNCHER_INTERFACE_TEST_H

#include "gtest/gtest.h"
#include <QTest>

#include "dbus/launcher_interface.h"

class ut_lanuncher_interface_test :public::testing::Test
{
public:
    ut_lanuncher_interface_test();
    virtual void SetUp() override;
    virtual void TearDown() override;

private:


};

#endif // UT_LANUNCHER_INTERFACE_TEST_H
