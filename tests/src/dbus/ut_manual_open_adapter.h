/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     wangmingliang <wangmingliang@uniontech.com>
* Maintainer: wangmingliang <wangmingliang@uniontech.com>
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
#ifndef UT_MANUAL_OPEN_ADAPTER_H
#define UT_MANUAL_OPEN_ADAPTER_H

#include "gtest/gtest.h"
#include <QTest>

class ManualOpenAdapter;
class ut_ManualOpenAdapter : public QObject
    , public testing::Test
{
    Q_OBJECT
public:
    ut_ManualOpenAdapter();
    virtual void SetUp() override;
    virtual void TearDown() override;
    ManualOpenAdapter *adapter = nullptr;

    bool isopen = false;
    bool isOpenTitle = false;
    bool isSearch = false;
    bool isShowManual = false;
public slots:
    void Open(const QString &app_name);
    void OpenTitle(const QString &app_name, const QString &title_name);
    void ShowManual(const QString &app_name);
    void Search(const QString &keyword);
};

#endif // UT_MANUAL_OPEN_ADAPTER_H
