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

#include "controller/search_db.h"

#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>

#include "base/consts.h"

namespace dman {

namespace {

const char kTableSchema[] = "CREATE TABLE IF NOT EXISTS search "
    "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "appName TEXT,"
    "anchor TEXT,"
    "content TEXT)";

const char kIndexSchema[] = "CREATE INDEX IF NOT EXISTS search_idx "
    "ON search "
    "(id,"
    "appName)";

const char kDeleteEntryByApp[] = "DELETE FROM search WHERE appName = ?";
const char kInsertEntry[] = "INSERT INTO search(appName, anchor, content) "
    "VALUES (?, ?, ?)";

QString GetDbName() {
  QDir cache_dir(GetCacheDir());
  cache_dir.mkpath(".");
  return cache_dir.absoluteFilePath("search_entry.db");
}

}  // namespace

SearchDb::SearchDb(QObject* parent)
    : QObject(parent),
      db_() {
  this->initConnections();
}

SearchDb::~SearchDb() {
  if (db_.isOpen()) {
    db_.close();
  }
}

void SearchDb::initConnections() {
  connect(this, &SearchDb::initDb,
          this, &SearchDb::handleInitDb);
  connect(this, &SearchDb::addSearchEntry,
          this, &SearchDb::handleAddSearchEntry);
  connect(this, &SearchDb::search,
          this, &SearchDb::handleSearch);
}

void SearchDb::handleInitDb() {
  db_ = QSqlDatabase::addDatabase("QSQLITE");
  const QString db_path = GetDbName();
  db_.setDatabaseName(db_path);
  if (!db_.open()) {
    qCritical() << "Failed to open search db:" << db_path;
    return;
  }

  QSqlQuery query(db_);
  if (!query.exec(kTableSchema)) {
    qCritical() << "Failed to initialize search table:"
                << query.lastError().text();
    return;
  }
  if (!query.exec(kIndexSchema)) {
    qCritical() << "Failed to create index for search table"
                << query.lastError().text();
  }
}

void SearchDb::handleAddSearchEntry(const QString& app_name,
                                    const QStringList& anchors,
                                    const QStringList& contents) {
  Q_ASSERT(db_.isOpen());
  Q_ASSERT(anchors.length() == contents.length());
  qDebug() << Q_FUNC_INFO << app_name;

  if (anchors.length() != contents.length()) {
    qCritical() << "anchor list and contents mismatch:"
                << anchors.length() << contents.length();
    return;
  }

  QSqlQuery query(db_);
  query.prepare(kDeleteEntryByApp);
  query.bindValue(0, app_name);
  if (!query.exec()) {
    qCritical() << "Failed to delete search entry:"
                << query.lastError().text();
    return;
  }

  query.prepare(kInsertEntry);
  bool ok = true;
  for (int i = 0; ok && (i < anchors.length()); ++i) {
    query.bindValue(0, app_name);
    query.bindValue(1, anchors.at(i));
    query.bindValue(2, contents.at(i));
    ok = query.exec();
  }

  if (!ok) {
    db_.rollback();
    qCritical() << "Failed to insert search entry:"
                << query.lastError().text();
  } else {
    db_.commit();
  }
}

void SearchDb::handleSearch(const QString& app_name, const QString& keyword) {
  qDebug() << Q_FUNC_INFO << keyword;
  SearchResultList result;
  if (app_name.isEmpty()) {
    // Global search

    if (true) {
      emit this->searchResult(app_name, keyword, result);
    }
  } else {
    // Search in app_name.
    if (true) {
      emit this->searchResult(app_name, keyword, result);
    }
  }
}

}  // namespace dman