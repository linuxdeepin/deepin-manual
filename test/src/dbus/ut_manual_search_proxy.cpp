#include "ut_manual_search_proxy.h"

#include "../src/dbus/manual_search_proxy.h"

ut_manual_search_proxy::ut_manual_search_proxy()
{

}

void ut_manual_search_proxy::SetUp()
{
    msp = new ManualSearchProxy();
}

void ut_manual_search_proxy::TearDown()
{
    delete msp;
}

TEST_F(ut_manual_search_proxy, connectToSender)
{
    msp->connectToSender();
}

TEST_F(ut_manual_search_proxy, RecvMsg)
{
    msp->RecvMsg("23423|close");
}

TEST_F(ut_manual_search_proxy, RecvMsg2)
{
    msp->RecvMsg("23423|open");
}

TEST_F(ut_manual_search_proxy, OnNewWindowOpen)
{
    msp->m_bWindowState = true;
    msp->OnNewWindowOpen("dde");
}

TEST_F(ut_manual_search_proxy, ManualExists)
{
    bool b = msp->ManualExists("dde");
//    ASSERT_EQ(b, true);
}

TEST_F(ut_manual_search_proxy, ManualExists2)
{
//    bool b = msp->ManualExists("dde");
//    ASSERT_EQ(b, false);
}
