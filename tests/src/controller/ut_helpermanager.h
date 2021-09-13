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
#ifndef UT_HELPERMANAGER_H
#define UT_HELPERMANAGER_H

#include "gtest/gtest.h"

#include <QObject>
#include <QTest>
#include <QWebEngineView>
#include <QStandardPaths>

class helperManager;

class ut_helperManager : public QObject, public::testing::Test
{
    Q_OBJECT
public:
    explicit ut_helperManager(QObject *parent = nullptr);
    ~ut_helperManager();
    virtual void SetUp() override;
    virtual void TearDown() override;
    helperManager *m_hm = nullptr;
    QWebEnginePage* stub_page();
    void stub_setWeb(QWebChannel * webchannel);
    QString stub_writableLocation(QStandardPaths::StandardLocation type);

    QWebChannel * webchannel = nullptr;

};

#endif // UT_HELPERMANAGER_H
