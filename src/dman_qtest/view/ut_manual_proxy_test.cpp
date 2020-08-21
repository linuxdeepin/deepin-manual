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
#include "ut_manual_proxy_test.h"

#define private public
#include "view/manual_proxy.h"
#undef private
#include "controller/config_manager.h"
#include "base/consts.h"

namespace dman {

ut_manual_proxy_test::ut_manual_proxy_test()
{

}

TEST_F(ut_manual_proxy_test, setApplicationState)
{
//    QSettings *setting = ConfigManager::getInstance()->getSettings();

//    setting->beginGroup(kConfigAppList);
//    QString appName("deepin-app-store");
//    if (setting->contains(appName)) {
//        setting->setValue(appName, true);

//        qDebug() << "deepin-app-store=" << setting->value(appName).toString();
//    }
//    setting->endGroup();

    ManualProxy mp;
//    setting->beginGroup(kConfigAppList);
//    ASSERT_TRUE(setting->value(appName).toBool());
    mp.setApplicationState("deepin-app-store");
//    ASSERT_FALSE(setting->value(appName).toBool());
//    setting->endGroup();
}

}
