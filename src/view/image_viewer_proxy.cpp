// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "view/image_viewer_proxy.h"
#include "view/widget/image_viewer.h"

#include <DLog>

ImageViewerProxy::ImageViewerProxy(ImageViewer *viewer,
                                   QObject *parent)
    : QObject(parent)
    , viewer_(viewer)
{
}

ImageViewerProxy::~ImageViewerProxy()
{
}

/**
 * @brief ImageViewerProxy::open
 * @param filepath 图片的绝对路径
 * 在窗口中显示图片
 */
void ImageViewerProxy::open(const QString &filepath)
{
    Q_ASSERT(viewer_ != nullptr);
    Q_UNUSED(filepath);
    viewer_->open(filepath);
}

/**
 * @brief ImageViewerProxy::openHttpUrl
 * @param httpUrl 链接地址
 * 打开外部链接
 */
void ImageViewerProxy::openHttpUrl(const QString &httpUrl)
{
    QDesktopServices::openUrl(QUrl(httpUrl));
}
