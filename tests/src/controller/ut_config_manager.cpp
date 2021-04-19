#include "ut_config_manager.h"

#include "controller/config_manager.h"
#include <QProcess>
#include <QStandardPaths>


ut_config_manager_test::ut_config_manager_test()
{

}


TEST_F(ut_config_manager_test, getSettings)
{
    ConfigManager *m_ap = new ConfigManager();
    m_ap->getSettings();
    delete m_ap;
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
