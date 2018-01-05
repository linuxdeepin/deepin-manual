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

#include "controller/search_manager.h"

namespace dman {


SearchManager::SearchManager(QObject* parent) : QObject(parent) {

}

SearchManager::~SearchManager() {

}

void SearchManager::addSearchEntry(const QString& app_name,
                                   const QString& index,
                                   const QString& content) {
  Q_UNUSED(app_name);
  Q_UNUSED(index);
  Q_UNUSED(content);
}

void SearchManager::search(const QString& keyword) {
  Q_UNUSED(keyword);
}

}  // namespace dman