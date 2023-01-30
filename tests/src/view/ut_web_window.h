// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UT_WEB_WINDOW_TEST_H
#define UT_WEB_WINDOW_TEST_H

#include "gtest/gtest.h"

#include <DObject>
#include <DApplicationHelper>

#include <QTest>
#include <QMainWindow>
#include <QWebChannel>
#include <QWebEnginePage>

DWIDGET_USE_NAMESPACE

class WebWindow;

class ut_web_window_test : public ::testing::Test
{
public:
    explicit ut_web_window_test();

    void stub_initweb();
    bool stub_isValid();
    bool stub_isValidfalse();
    void stub_setWeb(QWebChannel*);
    QWebEnginePage* stub_page();
    WebWindow *window = nullptr;
    DGuiApplicationHelper::ColorType stub_themeType() const;
    DGuiApplicationHelper::ColorType stub_themeTypeDark() const;
    QString stub_selectText() const;
    Qt::MouseButton stub_MouseButtonBack() const;
    Qt::MouseButton stub_MouseButtonForward() const;
    Qt::MouseButton stub_MouseButtonMiddle() const;
};

#endif // UT_WEB_WINDOW_TEST_H
