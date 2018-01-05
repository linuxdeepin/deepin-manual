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

#include <QDebug>
#include <QThread>

#include "controller/search_db.h"

namespace dman {

SearchManager::SearchManager(QObject* parent)
    : QObject(parent),
      db_(new SearchDb()),
      db_thread_(new QThread(this)) {
  db_thread_->start();
  db_->moveToThread(db_thread_);

  connect(db_thread_, &QThread::destroyed,
          db_, &QObject::deleteLater);
  emit db_->initDb();
}

SearchManager::~SearchManager() {
  db_thread_->quit();
  delete db_thread_;
  db_thread_ = nullptr;
}

void SearchManager::search(const QString& keyword) {
  qDebug() << Q_FUNC_INFO << keyword;
}

void SearchManager::setCurrentApp(const QString& app_name) {
  qDebug() << Q_FUNC_INFO << app_name;
}

void SearchManager::addSearchEntry(const QString& app_name,
                                   const QStringList& anchors,
                                   const QStringList& contents) {
  emit db_->addSearchEntry(app_name, anchors, contents);
}

}  // namespace dman