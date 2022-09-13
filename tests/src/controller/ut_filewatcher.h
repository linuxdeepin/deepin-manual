// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UT_FILEWATCHER_H
#define UT_FILEWATCHER_H

#include "gtest/gtest.h"
#include <QTest>

class fileWatcher;
class ut_fileWatcher : public::testing::Test
{
public:
    ut_fileWatcher();
    virtual void SetUp() override;
    virtual void TearDown() override;

    static QString stub_getSystemManualDir()
    {
        return "./manual-assets";
    }


protected:
    fileWatcher *m_fw = nullptr;
};

#endif // UT_FILEWATCHER_H
