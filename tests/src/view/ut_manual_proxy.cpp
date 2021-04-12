/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     wangmingliang <wangmingliang@uniontech.com>
* Maintainer: wangmingliang <wanmgmingliang@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ut_manual_proxy.h"

#include "view/manual_proxy.h"
#include "controller/config_manager.h"
#include "base/consts.h"
#include "../third-party/stub/stub.h"
#include <QProcess>
#include <QMutex>

ut_manual_proxy_test::ut_manual_proxy_test()
{

}

void ut_manual_proxy_test::SetUp()
{
    m_mp = new ManualProxy();
}

void ut_manual_proxy_test::TearDown()
{
    delete m_mp;
}

void stub_Handle()
{
}

TEST_F(ut_manual_proxy_test, getSystemManualDir)
{
    QString str = m_mp->getSystemManualDir();
    QString tmp = "deepin-manual/manual-assets";
    ASSERT_NE(str.indexOf(tmp), -1);
}

TEST_F(ut_manual_proxy_test, getSystemManualList)
{
    QStringList list = m_mp->getSystemManualList();

//    ASSERT_GE(list.count(), 1);
}


TEST_F(ut_manual_proxy_test, getUsedAppList)
{
    QStringList list = m_mp->getUsedAppList();

//    ASSERT_GE(list.count(), 1);
}


TEST_F(ut_manual_proxy_test, hasSelperSupport)
{
     m_mp->hasSelperSupport();

//    ASSERT_GE(list.count(), 1);
}

TEST_F(ut_manual_proxy_test, setApplicationState)
{

    QDir winInfoPath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    if (!winInfoPath.exists()) {
        winInfoPath.mkpath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    }
    QString winInfoFilePath(winInfoPath.filePath("wininfo-config.conf"));



    QDir configPath(winInfoFilePath);
    if (!configPath.exists()) {
        QProcess p;
        QString cmd = "cp ";
        //user目录
        cmd +=  QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        cmd += "/.config/deepin/deepin-manual/wininfo-config.conf ";
        cmd += QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        cmd += "/.config/deepin-manual_test/wininfo-config.conf";
        p.start(cmd);
        p.close();

        QSettings *setting = new QSettings(winInfoFilePath, QSettings::IniFormat);

        setting->beginGroup(kConfigAppList);
        QString appName("dde");
        if (setting->contains(appName)) {
            setting->setValue(appName, true);

            qDebug() << "dde=" << setting->value(appName).toString();
            ManualProxy mp;
            ASSERT_TRUE(setting->value(appName).toBool());
            qDebug() << "setting->value.dde.bool->>" << setting->value(appName).toBool();
            mp.setApplicationState(appName);
            ASSERT_FALSE(setting->value(appName).toBool());
            qDebug() << "setting->value.dde.bool->>" << setting->value(appName).toBool();
            setting->endGroup();
        }
         delete setting;
    } else {
        QSettings *setting = new QSettings(winInfoFilePath, QSettings::IniFormat);

        setting->beginGroup(kConfigAppList);
        QString appName("dde");
        if (setting->contains(appName)) {
            setting->setValue(appName, true);

            qDebug() << "dde=" << setting->value(appName).toString();
            ManualProxy mp;
            ASSERT_TRUE(setting->value(appName).toBool());
            qDebug() << "setting->value.dde.bool->>" << setting->value(appName).toBool();
            mp.setApplicationState(appName);
            ASSERT_FALSE(setting->value(appName).toBool());
            qDebug() << "setting->value.dde.bool->>" << setting->value(appName).toBool();
            setting->endGroup();
        }
         delete setting;
    }

}

TEST_F(ut_manual_proxy_test, setApplicationState2)
{

    QDir winInfoPath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    if (!winInfoPath.exists()) {
        winInfoPath.mkpath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    }
    QString winInfoFilePath(winInfoPath.filePath("wininfo-config.conf"));
    QDir configPath(winInfoFilePath);
    if (!configPath.exists()) {
        QProcess p;
        QString cmd = "cp ";
        //user目录
        cmd +=  QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        cmd += "/.config/deepin/deepin-manual/wininfo-config.conf ";
        cmd += QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        cmd += "/.config/deepin-manual_test/wininfo-config.conf";
        p.start(cmd);
        p.close();

        QSettings *setting = new QSettings(winInfoFilePath, QSettings::IniFormat);

        setting->beginGroup(kConfigAppList);
        QString appName("dde");
        if (setting->contains(appName)) {
            setting->setValue(appName, true);
            qDebug() << "dde=" << setting->value(appName).toString();
            ASSERT_TRUE(setting->value(appName).toBool());
            ManualProxy mp;
            QString strName("dde%2Findex%2F.html");
            mp.setApplicationState(strName);
            ASSERT_FALSE(setting->value(appName).toBool());
            setting->endGroup();
        }
        delete setting;
    } else {
        QSettings *setting = new QSettings(winInfoFilePath, QSettings::IniFormat);

        setting->beginGroup(kConfigAppList);
        QString appName("dde");
        if (setting->contains(appName)) {
            setting->setValue(appName, true);
            qDebug() << "dde=" << setting->value(appName).toString();
            ASSERT_TRUE(setting->value(appName).toBool());
            ManualProxy mp;
            QString strName("dde%2Findex%2F.html");
            mp.setApplicationState(strName);
            ASSERT_FALSE(setting->value(appName).toBool());
            setting->endGroup();
        }
        delete setting;
    }
}

TEST_F(ut_manual_proxy_test, supportClick)
{
    ManualProxy mp;
    mp.supportClick();
}
TEST_F(ut_manual_proxy_test, saveApplist)
{
    QStringList list;
    list << "dde";

    QDir winInfoPath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    if (!winInfoPath.exists()) {
        winInfoPath.mkpath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    }
    QString winInfoFilePath(winInfoPath.filePath("wininfo-config.conf"));
    QSettings *setting = new QSettings(winInfoFilePath, QSettings::IniFormat);
    setting->beginGroup(kConfigAppList);

    ManualProxy mp;

    if (setting->contains("dde")) {
        bool b = setting->value("dde").toBool();
        qDebug() << "dde.value-->" << b;
    } else {
        mp.saveAppList(list);
        ASSERT_TRUE(setting->value("dde").toBool());
    }

    QStringList list2;
    list2 << "dde_test";
    if (setting->contains("dde_test")) {
        bool b = setting->value("dde_test").toBool();
        qDebug() << "dde_test.value-->" << b;
    } else {
        mp.saveAppList(list2);
        ASSERT_TRUE(setting->value("dde_test").toBool());
    }
    setting->endGroup();
    delete setting;
}

TEST_F(ut_manual_proxy_test, showUpdateLabel)
{
}

TEST_F(ut_manual_proxy_test, appToPath)
{
    ManualProxy mp;
    QString path = mp.appToPath("ceshi");
    ASSERT_TRUE(path.compare("error") == 0);
}

TEST_F(ut_manual_proxy_test, getAppIconPath)
{
    ManualProxy mp;
    QString iconpaht = mp.getAppIconPath("dde-ceshi");
    ASSERT_TRUE(iconpaht.isEmpty());
}

TEST_F(ut_manual_proxy_test, getLocalAppName)
{
    ManualProxy mp;
    QString iconpaht = mp.getAppIconPath("dde-ceshi");
    ASSERT_TRUE(iconpaht.isEmpty());
}

TEST_F(ut_manual_proxy_test, getAppLocalDir)
{
    ManualProxy mp;
    QString iconpaht = mp.getAppLocalDir("/usr/share/deepin-manual/manual-assets/application/deepin-boot-maker/boot-maker1/");
    ASSERT_FALSE(iconpaht.isEmpty());
}
