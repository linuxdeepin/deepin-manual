// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_image_viewer_proxy.h"
#include "view/image_viewer_proxy.h"
#include "view/widget/image_viewer.h"
#include "resources/themes/images.h"


ut_image_viewer_proxy_test::ut_image_viewer_proxy_test()
{

}

void ut_image_viewer_proxy_test::SetUp()
{
    m_iv = new ImageViewer;
    m_ivp = new ImageViewerProxy(m_iv);
}

void ut_image_viewer_proxy_test::TearDown()
{
    delete m_ivp;
    delete m_iv;
}

TEST_F(ut_image_viewer_proxy_test, open)
{
    QString filePath = kImageDeepinManual;


    m_ivp->open(filePath);
    DLabel label;
    QPixmap pix(filePath);
    label.setPixmap(pix);
    label.setFixedSize(pix.size());
    ASSERT_EQ(m_ivp->viewer_->img_label_->width(), label.width());
}

TEST_F(ut_image_viewer_proxy_test, openHttpUrl)
{
    ImageViewer *iv = new ImageViewer;
    ImageViewerProxy ip(iv);
    ip.openHttpUrl("www.baidu.com");
}

