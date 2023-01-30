// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEEPIN_MANUAL_VIEW_IMAGE_VIEWER_PROXY_H
#define DEEPIN_MANUAL_VIEW_IMAGE_VIEWER_PROXY_H

#include <QObject>
#include <QDesktopServices>

class ImageViewer;

/**
 * @brief The ImageViewerProxy class
 * 双击放大图片接口类，供ｊｓ调用
 */
class ImageViewerProxy : public QObject
{
    Q_OBJECT
public:
    explicit ImageViewerProxy(ImageViewer *viewer, QObject *parent = nullptr);
    ~ImageViewerProxy() override;

public slots:
    void open(const QString &filepath);
    void openHttpUrl(const QString &httpUrl);

private:
    ImageViewer *viewer_ = nullptr;
};

#endif // DEEPIN_MANUAL_VIEW_IMAGE_VIEWER_PROXY_H
