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

#include "view/search_proxy.h"

dman::SearchProxy::SearchProxy(QObject* parent) : QObject(parent) {

}

dman::SearchProxy::~SearchProxy() {

}

void dman::SearchProxy::addSearchEntry(const QString& app_name,
                                       const QString& index,
                                       const QString& content) {
  Q_UNUSED(app_name);
  Q_UNUSED(index);
  Q_UNUSED(content);
}

void dman::SearchProxy::setCurrentAppName(const QString& app_name) {
  Q_UNUSED(app_name);
}
