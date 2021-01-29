#include "ut_manual_search_proxy.h"
#include "../src/dbus/manual_search_proxy.h"

#include "src/third-party/stub/stub.h"

#include <QDir>
#include <QFileInfoList>
#include <QtDBus/QtDBus>
#include <QtCore>

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

TEST_F(ut_manual_search_proxy, RecvMsg3)
{
    msp->RecvMsg("2323");
}

TEST_F(ut_manual_search_proxy, OnNewWindowOpen)
{
    msp->m_bWindowState = true;
    msp->OnNewWindowOpen("dde");
}

bool stub_isValid()
{
    return true;
}

TEST_F(ut_manual_search_proxy, OnNewWindowOpen2)
{
    msp->m_bWindowState = true;
    Stub st;
    st.set(ADDR(QDBusReply<void>, isValid), stub_isValid);
    msp->OnNewWindowOpen("dde");
    st.reset(ADDR(QDBusReply<void>, isValid));
}

TEST_F(ut_manual_search_proxy, ManualExists)
{
    bool b = msp->ManualExists("deepin");
    ASSERT_EQ(b, false);
}

QStringList stub_entryList(const QStringList &nameFilters, QDir::Filters filters, QDir::SortFlags sort)
{
    QStringList list;
    list << "system"
         << "application";

    return list;
}
TEST_F(ut_manual_search_proxy, ManualExists2)
{
    Stub st;
    st.set((QStringList(QDir::*)(const QStringList &, QDir::Filters, QDir::SortFlags) const)ADDR(QDir, entryList), stub_entryList);
    bool b = msp->ManualExists("dde");
    //    ASSERT_EQ(b, false);
}
