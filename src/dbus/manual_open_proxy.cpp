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

#include "dbus/manual_open_proxy.h"

#include <QDebug>

namespace {

QString ConvertOldDmanPath(const QString& app_name) {
  const QStringList parts = app_name.split('/');
  const int dman_index = parts.indexOf("dman");
  if (dman_index > 0 && dman_index < parts.length() - 1) {
    return parts.at(dman_index + 1);
  }
  return app_name;
}

}  // namespace

ManualOpenProxy::ManualOpenProxy(QObject* parent) : QObject(parent) {
  this->setObjectName("ManualOpenProxy");
}

ManualOpenProxy::~ManualOpenProxy() {

}

void ManualOpenProxy::Open(const QString& app_name) {
  const QString compact_app_name = ConvertOldDmanPath(app_name);
  qDebug() << Q_FUNC_INFO << app_name << compact_app_name;
  emit this->openManualRequested(compact_app_name);
}

void ManualOpenProxy::ShowManual(const QString& app_name) {
  this->Open(app_name);
}
