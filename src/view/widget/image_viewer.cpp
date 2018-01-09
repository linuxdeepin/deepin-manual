/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "view/widget/image_viewer.h"

#include <QDebug>
#include <QLabel>
#include <QResizeEvent>
#include <QStackedLayout>
#include <QtCore/QTimer>

#include "view/theme_manager.h"

namespace dman {

namespace {

const int kBorderSize = 12;
const int kCloseBtnSize = 24;

}  // namespace

ImageViewer::ImageViewer(QWidget* parent)
    : Dtk::Widget::DAbstractDialog(parent) {
  this->setObjectName("ImageViewer");
  this->initUI();

  connect(close_button_, &Dtk::Widget::DImageButton::clicked,
          this, &ImageViewer::close);
}

ImageViewer::~ImageViewer() {

}

void ImageViewer::open(const QString& filepath) {
  this->show();

  // Escape uri.
  QString abspath(filepath);
  if (abspath.contains("://")) {
    QUrl url(abspath);
    abspath = url.path();
  }

  const QPixmap pixmap(abspath);
  qDebug() << "pixmap size:" << pixmap.size();
  const int win_width = pixmap.width() + kBorderSize;
  const int win_height = pixmap.height() + kBorderSize;
  this->setFixedSize(win_width, win_height);
  // TODO(Shaohua): Resize image to fix screen.
  img_label_->setPixmap(pixmap);

  // Move close button to top-right corner of window.
  close_button_->move(pixmap.width() - kCloseBtnSize / 2, 0);
  close_button_->show();
  close_button_->raise();
}

void ImageViewer::initUI() {
  img_label_ = new QLabel();
  img_label_->setObjectName("ImageLabel");

  QStackedLayout* main_layout = new QStackedLayout();
  main_layout->setContentsMargins(0, 0, 0, 0);
  main_layout->setSpacing(0);
  main_layout->addWidget(img_label_);

  close_button_ = new Dtk::Widget::DImageButton(this);
  close_button_->setObjectName("CloseButton");
  close_button_->raise();
  close_button_->setFixedSize(kCloseBtnSize, kCloseBtnSize);

  this->setLayout(main_layout);
  this->setContentsMargins(kBorderSize, kBorderSize, kBorderSize, kBorderSize);

  ThemeManager::instance()->registerWidget(this);
}

}  // namespace dman