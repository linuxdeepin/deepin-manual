#include "ut_manual_filesupdate_proxy.h"
#include "../src/dbus/manual_filesupdate_proxy.h"

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
    QStringList list;
    list << "aa";
    list << "bb";
    mfuProxy->OnFilesUpdate(list);
}
