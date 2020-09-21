#include "ut_config_manager.h"

#define private public
#include "controller/config_manager.h"
#undef private
#include <QProcess>
#include <QStandardPaths>


ut_config_manager_test::ut_config_manager_test()
{

}

TEST_F(ut_config_manager_test, getWinInfoConfigPath)
{
    QDir winInfoPath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    if (winInfoPath.exists()) {
        QProcess p;
        p.start("rm -rf /home/kevin_w/.config/deepin-manual_test");
        //p.start("mv /home/kevin_w/.config/deepin/deepin-manual/ /home/kevin_w/.config/deepin/deepin-manual-test");
        p.close();
    }
    ConfigManager cm;
    QString strPath =  cm.getWinInfoConfigPath();
    ASSERT_EQ(strPath, "/home/kevin_w/.config/deepin-manual_test/wininfo-config.conf");

    QProcess p;
    p.start("cp /home/kevin_w/.config/deepin/deepin-manual/wininfo-config.conf /home/kevin_w/.config/deepin-manual_test/wininfo-config.conf");
    p.close();
}
