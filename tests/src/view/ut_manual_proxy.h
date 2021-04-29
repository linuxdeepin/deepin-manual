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
#ifndef UT_MANUAL_PROXY_TEST_H
#define UT_MANUAL_PROXY_TEST_H

#include "gtest/gtest.h"
#include <QTest>
#include <DSysInfo>

#include <QtGui/private/qiconloader_p.h>

class ManualProxy;
class ut_manual_proxy_test : public::testing::Test
{
public:
    ut_manual_proxy_test();
    virtual void SetUp() override;
    virtual void TearDown() override;
    ManualProxy *m_mp = nullptr;
    void stub_Handle();

    bool stub_exists() const;
    QStringList stub_entryList(QDir::Filters f, QDir::SortFlags s) const;
    bool stub_fileopen(QFile::OpenMode);
    QString stub_LocalNamezh_HK();


     QString stub_LocalNamezh_TW();


    QString stub_LocalNameug_CN();


    QString stub_LocalNameug_eu();


    QString stub_LocalNameug_bo_CN();

    static Dtk::Core::DSysInfo::DeepinType stub_deepinTypeDeepinPersonal()
    {
        return Dtk::Core::DSysInfo::DeepinPersonal;
    }

    static Dtk::Core::DSysInfo::DeepinType stub_deepinTypeDeepinServer()
    {
        return Dtk::Core::DSysInfo::DeepinServer;
    }

    static Dtk::Core::DSysInfo::DeepinType stub_deepinTypeDeepinUnknownDeepin()
    {
        return Dtk::Core::DSysInfo::UnknownDeepin;
    }

    static bool stub_qstringcontains(const QString& s, Qt::CaseSensitivity s1)
    {
        return true;
    }

    static int stub_listsize()
    {
        return 1;
    }

    static bool stub_deepinTypeDeepinCommunity()
    {
        return true;
    }

    QThemeIconInfo stub_loadIcon(const QString &iconname) const;
};

#endif // UT_MANUAL_PROXY_TEST_H
