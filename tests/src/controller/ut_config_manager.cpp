#include "ut_config_manager.h"

#include "controller/config_manager.h"
#include <QProcess>
#include <QStandardPaths>


ut_config_manager_test::ut_config_manager_test()
{

}


void ut_config_manager_test::SetUp()
{
    m_ap = new ConfigManager();
}

void ut_config_manager_test::TearDown()
{
    delete m_ap;
}

TEST_F(ut_config_manager_test, getSettings)
{
    m_ap->getSettings();
}

TEST_F(ut_config_manager_test, getWinInfoConfigPath)
{
    QProcess pro(0);
    QString command = "whoami";
    QStringList args;
    pro.start(command, args);
    pro.waitForFinished();
    QString strTemp = QString::fromLocal8Bit(pro.readAllStandardOutput());
    QString LoginUser = strTemp.trimmed();


    ConfigManager cm;
    QString strPath =  cm.getWinInfoConfigPath();

    ASSERT_FALSE(strPath.isEmpty());
}
