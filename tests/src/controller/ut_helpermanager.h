// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
