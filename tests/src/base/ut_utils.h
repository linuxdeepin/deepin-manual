// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UT_UTILS_TEST_H
#define UT_UTILS_TEST_H

#include "gtest/gtest.h"

#include <DApplicationHelper>
#include <DSysInfo>

#include <QTest>
#include <QObject>
#include <QTimer>

class Utils;

class ut_utils_test : public QObject, public::testing::Test
{
    Q_OBJECT
public:
    ut_utils_test();

    static Dtk::Core::DSysInfo::DeepinType stub_deepinTypeServer()
    {
        return Dtk::Core::DSysInfo::DeepinServer;
    }

    static Dtk::Core::DSysInfo::DeepinType stub_deepinTypeDeepinPersonal()
    {
        return Dtk::Core::DSysInfo::DeepinPersonal;
    }

    static Dtk::Core::DSysInfo::DeepinType stub_deepinTypeDeepinUnknownDeepin()
    {
        return Dtk::Core::DSysInfo::UnknownDeepin;
    }

    static QString stub_LocalNamezh_HK()
    {
        return "zh_HK";
    }

    static QString stub_LocalNamezh_TW()
    {
        return "zh_TW";
    }

    static QString stub_LocalNameug_CN()
    {
        return "ug_CN";
    }

    static QString stub_LocalNameug_EU()
    {
        return "en_us";
    }

    static bool stub_isValid();

    static bool stub_deepinTypeDeepinCommunity()
    {
        return true;
    }

    static QByteArray stub_readAllStandardOutput()
    {
        QByteArray bytes("Loongson");
        return bytes;
    }

    bool stub_exists() const;

private slots:

};

#endif // UT_UTILS_TEST_H
