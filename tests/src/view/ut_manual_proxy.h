// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UT_MANUAL_PROXY_TEST_H
#define UT_MANUAL_PROXY_TEST_H

#include "gtest/gtest.h"

#include <DSysInfo>

#include <QStandardPaths>
#include <QTest>
// #include <QtGui/private/qiconloader_p.h>

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

    QString stub_writableLocation(QStandardPaths::StandardLocation type);

    static Dtk::Core::DSysInfo::DeepinType stub_deepinTypeDeepinPersonal()
    {
        return Dtk::Core::DSysInfo::DeepinPersonal;
    }

    static Dtk::Core::DSysInfo::DeepinType stub_deepinTypeDeepinServer()
    {
        return Dtk::Core::DSysInfo::DeepinServer;
    }

    static Dtk::Core::DSysInfo::UosEdition stub_deepinTypeUosEnterprise()
    {
        return Dtk::Core::DSysInfo::UosEnterprise;
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

    // QThemeIconInfo stub_loadIcon(const QString &iconname) const;
};

#endif // UT_MANUAL_PROXY_TEST_H
