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
    QString stub_selectText() const;
    Qt::MouseButton stub_MouseButtonBack() const;
    Qt::MouseButton stub_MouseButtonForward() const;
    Qt::MouseButton stub_MouseButtonMiddle() const;
};

#endif // UT_WEB_WINDOW_TEST_H
