#include "ut_manual_filesupdate_proxy.h"
#include "../src/dbus/manual_filesupdate_proxy.h"

#include <QSignalSpy>

ut_ManualFilesUpdateProxy::ut_ManualFilesUpdateProxy()
{
}

void ut_ManualFilesUpdateProxy::SetUp()
{
    mfuProxy = new ManualFilesUpdateProxy();
}

void ut_ManualFilesUpdateProxy::TearDown()
{
    delete mfuProxy;
}

TEST_F(ut_ManualFilesUpdateProxy, OnFilesUpdate)
{
    QSignalSpy spy(mfuProxy, SIGNAL(FilesUpdate(const QStringList &)));
    QStringList list;
    list << "aa";
    list << "bb";
    mfuProxy->OnFilesUpdate(list);
    ASSERT_EQ(spy.count(), 1);
    QList<QVariant> argu = spy.takeFirst();
    ASSERT_EQ(argu.at(0).toStringList(), list);
}
