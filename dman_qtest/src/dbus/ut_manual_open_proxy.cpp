#include "ut_manual_open_proxy.h"

#include "../src/dbus/manual_open_proxy.h"

ut_manual_open_proxy::ut_manual_open_proxy()
{

}

void ut_manual_open_proxy::SetUp()
{
    mopen = new ManualOpenProxy();
}

void ut_manual_open_proxy::TearDown()
{
    delete mopen;
}

TEST_F(ut_manual_open_proxy, Open)
{
    mopen->Open("dde");
}

TEST_F(ut_manual_open_proxy, OpenTitle)
{
    mopen->OpenTitle("dde", "概述");
}

TEST_F(ut_manual_open_proxy, ShowManual)
{
    mopen->ShowManual("dde");
}

TEST_F(ut_manual_open_proxy, Search)
{
    mopen->Search("应用");
}
