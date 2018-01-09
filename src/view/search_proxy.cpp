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

#include <QDebug>

#include "controller/search_manager.h"

dman::SearchProxy::SearchProxy(SearchManager* manager, QObject* parent)
    : QObject(parent),
      manager_(manager) {
  Q_ASSERT(manager_ != nullptr);
}

dman::SearchProxy::~SearchProxy() {

}

void dman::SearchProxy::setCurrentApp(const QString& app_name) {
  current_app_ = app_name;
  emit this->currentAppChanged(app_name);
}

void dman::SearchProxy::addSearchEntry(const QString& app_name,
                                       const QStringList& anchors,
                                       const QStringList& contents) {
  qDebug() << Q_FUNC_INFO << anchors;
  emit manager_->addSearchEntry(app_name, anchors, contents);
}
