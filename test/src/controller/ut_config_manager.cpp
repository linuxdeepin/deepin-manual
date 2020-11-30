#include "ut_config_manager.h"

#include "controller/config_manager.h"
#include <QProcess>
#include <QStandardPaths>


ut_config_manager_test::ut_config_manager_test()
{

}

TEST_F(ut_config_manager_test, getWinInfoConfigPath)
{
//    QProcess pro(0);
//    QString command = "whoami";
//    QStringList args;
//    pro.start(command, args);
//    pro.waitForFinished();
//    QString strTemp = QString::fromLocal8Bit(pro.readAllStandardOutput());
//    QString LoginUser = strTemp.trimmed();

//    QDir winInfoPath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
//    if (winInfoPath.exists()) {
//        QString comm;
//        comm = "rm -rf /home/"
//               + LoginUser
//               + "/.config/deepin-manual_test";
//        QProcess p;
//        p.start(comm);
//        p.close();
//    }
//    ConfigManager cm;
//    QString strPath =  cm.getWinInfoConfigPath();
//    QString configPath = "/home/"
//                         + LoginUser
//                         + "/.config/deepin-manual_test/wininfo-config.conf";
//    ASSERT_EQ(strPath, configPath);

//    QProcess p;
//    QString strManualPath;
//    strManualPath = "/home/"
//                    + LoginUser
//                    + "/.config/deepin/deepin-manual/wininfo-config.conf";
//    QString strTestPath;
//    strTestPath = "/home/"
//                  + LoginUser
//                  + ".config/deepin-manual_test/wininfo-config.conf";
//    p.start("cp " + strManualPath + " " + strTestPath);
//    p.close();
}
