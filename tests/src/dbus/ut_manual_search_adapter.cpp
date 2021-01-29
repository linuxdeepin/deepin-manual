#include "ut_manual_search_adapter.h"
#include "../src/dbus/manual_search_adapter.h"
#include <QWidget>

ut_ManualSearchAdapter::ut_ManualSearchAdapter()
{
}

void ut_ManualSearchAdapter::SetUp()
{
    wiget = new QWidget();
    msAdapter = new ManualSearchAdapter(wiget);
}

void ut_ManualSearchAdapter::TearDown()
{
    delete msAdapter;
    delete wiget;
}

TEST_F(ut_ManualSearchAdapter, ManualExists)
{
    bool b = msAdapter->ManualExists("aaa");
    ASSERT_FALSE(b);
}

TEST_F(ut_ManualSearchAdapter, OnNewWindowOpen)
{
    msAdapter->OnNewWindowOpen("deepin");
}
