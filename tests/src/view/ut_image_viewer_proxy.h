// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
