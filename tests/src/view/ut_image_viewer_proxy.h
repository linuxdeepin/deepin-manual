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
#ifndef UT_IMAGE_VIEWER_PROXY_TEST_H
#define UT_IMAGE_VIEWER_PROXY_TEST_H

#include "gtest/gtest.h"
#include <QTest>

class ImageViewerProxy;
class ImageViewer;
class ut_image_viewer_proxy_test : public::testing::Test
{
public:
    ut_image_viewer_proxy_test();
    virtual void SetUp() override;
    virtual void TearDown() override;
    ImageViewerProxy *m_ivp = nullptr;
    ImageViewer *m_iv = nullptr;
};

//}
#endif // UT_IMAGE_VIEWER_PROXY_TEST_H
