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
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "base/consts.h"
#include "cppjieba/Jieba.hpp"

namespace dman {

namespace {

const char kTableSchema[] = "CREATE TABLE IF NOT EXISTS search "
    "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "appName TEXT,"
    "anchor TEXT,"
    "content TEXT,"
    "words TEXT)";

const char kIndexSchema[] = "CREATE INDEX IF NOT EXISTS search_idx "
    "ON search "
    "(id,"
    "appName)";

const char kDeleteEntryByApp[] = "DELETE FROM search WHERE appName = ?";
const char kInsertEntry[] = "INSERT INTO search "
    "(appName, anchor, content, words) "
    "VALUES (?, ?, ?, ?)";

const char kSelectAll[] = "SELECT * FROM search";
const char kSelectApp[] = "SELECT * FROM search WHERE appName = ?";

const int kResultLimitation = 10;

const char kJiebaDict[] = JIEBA_DICT "/jieba.dict.utf8";
const char kHmmDict[] = JIEBA_DICT "/hmm_model.utf8";
const char kUserDict[] = JIEBA_DICT "/user.dict.utf8";
const char kIdfFile[] = JIEBA_DICT "/idf.utf8";
const char kStopWords[] = JIEBA_DICT "/stop_words.utf8";

QString GetDbName() {
  QDir cache_dir(GetCacheDir());
  cache_dir.mkpath(".");
  return cache_dir.absoluteFilePath("search_entry.db");
}

}  // namespace

struct SearchDbPrivate {
  QSqlDatabase db;
  cppjieba::Jieba* jieba = nullptr;

  // Key => contents_words pair
  QHash<QString, QString> cache;
};

SearchDb::SearchDb(QObject* parent)
    : QObject(parent),
      p_(new SearchDbPrivate()) {
  qRegisterMetaType<SearchResult>("SearchResult");
  qRegisterMetaType<SearchResultList>("SearchResultList");
  this->initConnections();
  p_->jieba = new cppjieba::Jieba(kJiebaDict,
                                  kHmmDict,
                                  kUserDict,
                                  kIdfFile,
                                  kStopWords);
}

SearchDb::~SearchDb() {
  if (p_ != nullptr) {
    if (p_->db.isOpen()) {
      p_->db.close();
    }
    if (p_->jieba != nullptr) {
      delete p_->jieba;
      p_->jieba = nullptr;
    }
    delete p_;
    p_ = nullptr;
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

bool SearchDb::IsKeywordMatch(const QString& key,
                              const QString& words,
                              const QString& keyword) {
  if (!p_->cache.contains(key)) {
    p_->cache.insert(key, words);
  }

  return p_->cache.value(key).indexOf(keyword) > -1;
}

void SearchDb::handleInitDb() {
  p_->db = QSqlDatabase::addDatabase("QSQLITE");
  const QString db_path = GetDbName();
  p_->db.setDatabaseName(db_path);
  if (!p_->db.open()) {
    qCritical() << "Failed to open search db:" << db_path;
    return;
  }

  QSqlQuery query(p_->db);
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
  Q_ASSERT(p_->db.isOpen());
  Q_ASSERT(anchors.length() == contents.length());
  qDebug() << Q_FUNC_INFO << app_name;

  if (anchors.length() != contents.length()) {
    qCritical() << "anchor list and contents mismatch:"
                << anchors.length() << contents.length();
    return;
  }

  QSqlQuery query(p_->db);
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
    const std::string content_std(contents.at(i).toLower().toStdString());
    std::vector<std::string> word_list;
    p_->jieba->CutForSearch(content_std, word_list);
    const std::string words_std = limonp::Join(word_list.begin(),
                                               word_list.end(),
                                               "/");
    const QString words = QString::fromStdString(words_std);
    const QString key = app_name + anchors.at(i);
    // Add to memory cache.
    p_->cache.insert(key, words);

    // Save to database.
    query.bindValue(0, app_name);
    query.bindValue(1, anchors.at(i));
    query.bindValue(2, contents.at(i));
    query.bindValue(3, words);
    ok = query.exec();
  }

  if (!ok) {
    p_->db.rollback();
    qCritical() << "Failed to insert search entry:"
                << query.lastError().text();
  } else {
    p_->db.commit();
  }
}

void SearchDb::handleSearch(const QString& app_name, const QString& keyword) {
  qDebug() << Q_FUNC_INFO << keyword;
  Q_ASSERT(p_->db.isOpen());

  SearchResultList result;
  QSqlQuery query(p_->db);

  if (app_name.isEmpty()) {
    // Global search
    query.prepare(kSelectAll);
  } else {
    // Search in app_name.
    query.prepare(kSelectApp);
    query.bindValue(0, app_name);
  }
  if (query.exec()) {
    while (query.next() && (result.size() < kResultLimitation)) {
      const QString words = query.value(4).toString();
      const QString key = query.value(1).toString() + query.value(2).toString();
      if (this->IsKeywordMatch(key, words, keyword)) {
        result.append({
                          query.value(1).toString(),
                          query.value(2).toString(),
                      });
      }
    }
  } else {
    qCritical() << "Failed to select app content:"
                << query.lastError().text();
  }

  qDebug() << "result size:" << result.size() << keyword << app_name;

  emit this->searchResult(app_name, keyword, result);
}

}  // namespace dman