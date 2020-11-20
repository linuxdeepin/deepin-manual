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
#include <QProcess>
#include <QMutex>

namespace dman {

ut_manual_proxy_test::ut_manual_proxy_test()
{

}

TEST_F(ut_manual_proxy_test, openExternalLink)
{
    ManualProxy mp;
    //mp.openExternalLink("//===home/kevin_w/project/qtest/build-manual-unknown-Debug/coverageResult/report/home/kevin_w/project/qtest/manual/src/view/index.html");
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
        p.start("cp /home/kevin_w/.config/deepin/deepin-manual/wininfo-config.conf /home/kevin_w/.config/deepin-manual_test/wininfo-config.conf");
        p.close();
    } else {
        QSettings *setting = new QSettings(winInfoFilePath, QSettings::IniFormat);

        setting->beginGroup(kConfigAppList);
//    setting->setValue("dde", true);
        QString appName("dde");
        if (setting->contains(appName)) {
            setting->setValue(appName, true);

            qDebug() << "dde=" << setting->value(appName).toString();
        }
//    setting->endGroup();

        ManualProxy mp;
//    setting->beginGroup(kConfigAppList);
        ASSERT_TRUE(setting->value(appName).toBool());
        qDebug() << "setting->value.dde.bool->>" << setting->value(appName).toBool();
        mp.setApplicationState(appName);
        ASSERT_FALSE(setting->value(appName).toBool());
        qDebug() << "setting->value.dde.bool->>" << setting->value(appName).toBool();
        setting->endGroup();
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
        p.start("cp /home/kevin_w/.config/deepin/deepin-manual/wininfo-config.conf "
                "/home/kevin_w/.config/deepin-manual_test/wininfo-config.conf");
        p.close();
    } else {
        QSettings *setting = new QSettings(winInfoFilePath, QSettings::IniFormat);

        setting->beginGroup(kConfigAppList);
        QString appName("dde");
        if (setting->contains(appName)) {
            setting->setValue(appName, true);

            qDebug() << "dde=" << setting->value(appName).toString();
        }
        ASSERT_TRUE(setting->value(appName).toBool());
        ManualProxy mp;
        QString strName("dde%2Findex%2F.html");
        mp.setApplicationState(strName);
        ASSERT_FALSE(setting->value(appName).toBool());
        setting->endGroup();
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
}



}
