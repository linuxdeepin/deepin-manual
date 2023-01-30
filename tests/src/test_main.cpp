// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

// test_main.cpp 测试入口

#include <gtest/gtest.h>

#include <QTest>
#include <QApplication>

#if defined(CMAKE_SAFETYTEST_ARG_ON)
#include <sanitizer/asan_interface.h>
#endif

QT_BEGIN_NAMESPACE
QTEST_ADD_GPU_BLACKLIST_SUPPORT_DEFS
QT_END_NAMESPACE

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QApplication app(argc, argv);
    QTEST_DISABLE_KEYPAD_NAVIGATION
    QTEST_ADD_GPU_BLACKLIST_SUPPORT

    testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();

#ifdef ENABLE_UI_TEST
    app.exec();
#endif
    QTEST_SET_MAIN_SOURCE_PATH

#ifdef CMAKE_SAFETYTEST_ARG_ON
    __sanitizer_set_report_path("asan.log");
#endif
    return ret;
}
