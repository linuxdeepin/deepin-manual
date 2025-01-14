// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_image_viewer.h"
#include "resources/themes/images.h"
#include "view/widget/image_viewer.h"
#include "src/third-party/stub/stub.h"

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
#include <QDesktopWidget>
#else
#include <QScreen>
#endif
#include <QImageReader>

ut_image_viewer_test::ut_image_viewer_test()
{

}

bool imageread()
{
    return  false;
}

TEST_F(ut_image_viewer_test, open)
{
    QString filePath = kImageDeepinManual;
    ImageViewer iv;
    iv.open(filePath);
    DLabel label;
    QPixmap pix(filePath);
    label.setPixmap(pix);
    label.setFixedSize(pix.size());
    ASSERT_EQ(iv.img_label_->width(), label.width());

     iv.open(filePath+"://");

     Stub st;
     st.set((bool (QImageReader::*)(QImage *))ADDR(QImageReader, read), imageread);
     iv.open(filePath);

     ASSERT_EQ(iv.img_label_->width(), label.width());
}


TEST_F(ut_image_viewer_test, open2)
{
    QString filePath = "/home/kevin_w/Pictures/Glossy-Gradient.jpg";
    ImageViewer iv;
//    iv.initUI();
    iv.open(filePath);
    QPixmap pix(filePath);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    const QRect screen_rect = qApp->desktop()->screenGeometry(QCursor::pos());
#else
    const QRect screen_rect = QGuiApplication::primaryScreen()->geometry();
#endif
    const int pixmap_max_width = static_cast<int>(screen_rect.width() * 0.8);
    const int pixmap_max_height = static_cast<int>(screen_rect.height() * 0.8);
    pix = pix.scaled(pixmap_max_width, pixmap_max_height, Qt::KeepAspectRatio,
                     Qt::SmoothTransformation);
    ASSERT_TRUE(iv.img_label_->width() != pix.width());
}
TEST_F(ut_image_viewer_test, initUi)
{
    ImageViewer iv;
    iv.initUI();
    QTest::keyPress(iv.window(), Qt::Key_Escape);
    ASSERT_TRUE(iv.img_label_);
    ASSERT_TRUE(iv.close_button_);
    ASSERT_EQ(iv.img_label_->objectName(), "ImageLabel");
}

TEST_F(ut_image_viewer_test, mousePressEvent)
{
    QString filePath = "/home/kevin_w/Pictures/Glossy-Gradient.jpg";
    ImageViewer iv;
//    iv.initUI();
    iv.open(filePath);
    QTest::mouseClick(iv.window(), Qt::LeftButton);
    ASSERT_FALSE(iv.isVisible());
    ASSERT_FALSE(iv.isActiveWindow());
    QMouseEvent *evnReleaseEnter;
    evnReleaseEnter = new QMouseEvent( QEvent::MouseButtonRelease, QPoint(0, 0), Qt::RightButton, Qt::NoButton, Qt::NoModifier );
    iv.mousePressEvent(evnReleaseEnter);
    delete evnReleaseEnter;
    ASSERT_FALSE(iv.isVisible());
}


TEST_F(ut_image_viewer_test, paintEvent)
{
    ImageViewer iv;
    iv.setFixedSize(600, 600);
    QPixmap pixmap(iv.size());
    iv.render(&pixmap);
    EXPECT_EQ(pixmap.size(), iv.size());
}
