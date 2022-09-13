// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UT_MANUAL_SEARCH_ADAPTER_H
#define UT_MANUAL_SEARCH_ADAPTER_H

#include "gtest/gtest.h"
#include <QTest>

class ManualSearchAdapter;

class ut_ManualSearchAdapter : public QObject
    , public testing::Test
{
    Q_OBJECT
public:
    ut_ManualSearchAdapter();
    virtual void SetUp() override;
    virtual void TearDown() override;
    ManualSearchAdapter *msAdapter = nullptr;
    bool isOnNewWindowOpen = false;
public slots:
    void OnNewWindowOpen(const QString &data);
};

#endif // UT_MANUAL_SEARCH_ADAPTER_H
