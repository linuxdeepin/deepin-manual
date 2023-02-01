// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UT_MANUAL_FILESUPDATE_ADAPTER_H
#define UT_MANUAL_FILESUPDATE_ADAPTER_H

#include "gtest/gtest.h"
#include <QTest>

class ManualFilesUpdateAdapter;
class ut_ManualFilesUpdateAdapter : public QObject
    , public testing::Test
{
    Q_OBJECT
public:
    ut_ManualFilesUpdateAdapter();
    virtual void SetUp() override;
    virtual void TearDown() override;
    ManualFilesUpdateAdapter *adapter = nullptr;
    bool isOnFilesUpdate = false;
public slots:
    void OnFilesUpdate(const QStringList &list);
};

#endif // UT_MANUAL_FILESUPDATE_ADAPTER_H
