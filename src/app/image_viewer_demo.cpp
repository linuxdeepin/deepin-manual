// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QApplication>

#include "view/widget/image_viewer.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    qDebug() << "QApplication initialized";

    qDebug() << "Creating ImageViewer instance";
    dman::ImageViewer viewer;
    viewer.open("/tmp/a.jpg");

    return app.exec();
}
