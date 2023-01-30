// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
    QSettings *qs = m_ap->getSettings();
    ASSERT_TRUE(qs != nullptr);
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
