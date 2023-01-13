#include "ut_manual_search_adapter.h"
#include "../src/dbus/manual_search_adapter.h"

#include <QWidget>

ut_ManualSearchAdapter::ut_ManualSearchAdapter()
{
}

void ut_ManualSearchAdapter::OnNewWindowOpen(const QString &data)
{
    isOnNewWindowOpen = true;
}

void ut_ManualSearchAdapter::SetUp()
{
    msAdapter = new ManualSearchAdapter(this);
}

void ut_ManualSearchAdapter::TearDown()
{
    delete msAdapter;
}

TEST_F(ut_ManualSearchAdapter, ManualExists)
{
    bool b = msAdapter->ManualExists("aaa");
    ASSERT_FALSE(b);
}

TEST_F(ut_ManualSearchAdapter, OnNewWindowOpen)
{
    msAdapter->OnNewWindowOpen("deepin");
    ASSERT_TRUE(isOnNewWindowOpen);
}
