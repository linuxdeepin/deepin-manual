// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEEPIN_MANUAL_VIEW_WIDGETS_IMAGE_VIEWER_H
#define DEEPIN_MANUAL_VIEW_WIDGETS_IMAGE_VIEWER_H

#include <DLabel>
#include <DDialogCloseButton>

#include <QDialog>

DWIDGET_USE_NAMESPACE

/**
 * @brief ImageViewer::ImageViewer
 * @param parent
 * 用于帮助手册中浏览帮助内容时，点击大图后展示全屏的图片
 */
class ImageViewer : public QDialog
{
    Q_OBJECT
public:
    explicit ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer() override;

public slots:
    void open(const QString &filepath);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void initUI();

    DLabel *img_label_ = nullptr;
    DDialogCloseButton *close_button_;

    using QDialog::open;
};

#endif // DEEPIN_MANUAL_VIEW_WIDGETS_IMAGE_VIEWER_H
