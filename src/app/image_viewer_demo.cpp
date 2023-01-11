// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QApplication>

#include "view/widget/image_viewer.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    dman::ImageViewer viewer;
    viewer.open("/tmp/a.jpg");

    return app.exec();
}