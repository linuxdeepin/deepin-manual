// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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

bool isSendWinInfo = false;

bool ut_QDBusConnection_connect(const QString &, const QString &, const QString &, const QString &, QObject *, const char *)
{
    isSendWinInfo = true;
    return true;
}

TEST_F(ut_manual_search_proxy, connectToSender)
{
    Stub st;
    st.set((bool (QDBusConnection::*)(const QString &, const QString &, const QString &, const QString &, QObject *, const char *))ADDR(QDBusConnection, connect),
           ut_QDBusConnection_connect);
    msp->connectToSender();
    ASSERT_TRUE(isSendWinInfo);
}

TEST_F(ut_manual_search_proxy, RecvMsg)
{
    msp->RecvMsg("23423|close");
    ASSERT_FALSE(msp->m_bWindowState);
    msp->RecvMsg("23423|open");
    ASSERT_TRUE(msp->m_bWindowState);
    msp->RecvMsg("2323");
}

bool stub_isValid()
{
    return true;
}

bool ut_ActivateWindow = false;

QDBusMessage ut_call(QDBus::CallMode mode,
                     const QString &method,
                     const QVariant &arg1 = QVariant(),
                     const QVariant &arg2 = QVariant(),
                     const QVariant &arg3 = QVariant(),
                     const QVariant &arg4 = QVariant(),
                     const QVariant &arg5 = QVariant(),
                     const QVariant &arg6 = QVariant(),
                     const QVariant &arg7 = QVariant(),
                     const QVariant &arg8 = QVariant())
{
    ut_ActivateWindow = true;
    QDBusMessage busmsg;
    return busmsg;
}

TEST_F(ut_manual_search_proxy, OnNewWindowOpen)
{
    msp->m_bWindowState = true;
    msp->OnNewWindowOpen("dde");

    Stub st;
    st.set(ADDR(QDBusReply<void>, isValid), stub_isValid);

    st.set((QDBusMessage(QDBusAbstractInterface::*)(QDBus::CallMode, const QString &,
                                                    const QVariant &, const QVariant &, const QVariant &, const QVariant &,
                                                    const QVariant &, const QVariant &, const QVariant &, const QVariant &))ADDR(QDBusAbstractInterface, call),
           ut_call);

    msp->OnNewWindowOpen("dde");
    ASSERT_TRUE(ut_ActivateWindow);
    st.reset(ADDR(QDBusReply<void>, isValid));
    st.reset((QDBusMessage(QDBusAbstractInterface::*)(QDBus::CallMode, const QString &,
                                                      const QVariant &, const QVariant &, const QVariant &, const QVariant &,
                                                      const QVariant &, const QVariant &, const QVariant &, const QVariant &))ADDR(QDBusAbstractInterface, call));
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
    ASSERT_EQ(b, false);
}
