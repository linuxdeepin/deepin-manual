#include "ut_settings_proxy.h"

#include "view/settings_proxy.h"



ut_settings_proxy_test::ut_settings_proxy_test()
{

}

TEST_F(ut_settings_proxy_test, openUrl)
{
    SettingsProxy sp;
    sp.openUrl("===/home/kevin_w/project/qtest/build-manual-unknown-Debug/coverageResult/report/home/kevin_w/project/qtest/manual/src/view/index.html");
}
