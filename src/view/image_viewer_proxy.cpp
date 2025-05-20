// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "view/image_viewer_proxy.h"
#include "view/widget/image_viewer.h"
#include "base/ddlog.h"

#include <DLog>
#include <QUrl>

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
    qCDebug(app) << "Opening image file:" << filepath;
    viewer_->open(filepath);
}

/**
 * @brief ImageViewerProxy::openHttpUrl
 * @param httpUrl 链接地址
 * 打开外部链接
 */
void ImageViewerProxy::openHttpUrl(const QString &httpUrl)
{
    qCDebug(app) << "Opening external URL:" << httpUrl;
    if (!QDesktopServices::openUrl(QUrl(httpUrl))) {
        qCWarning(app) << "Failed to open URL:" << httpUrl;
    }
}
