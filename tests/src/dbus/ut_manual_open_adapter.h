// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
