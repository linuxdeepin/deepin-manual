/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DEEPIN_MANUAL_VIEW_IMAGE_VIEWER_PROXY_H
#define DEEPIN_MANUAL_VIEW_IMAGE_VIEWER_PROXY_H

#include <QObject>
#include <QDesktopServices>

namespace dman {

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

} // namespace dman

#endif // DEEPIN_MANUAL_VIEW_IMAGE_VIEWER_PROXY_H
