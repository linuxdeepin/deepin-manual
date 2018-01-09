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

  // { "appName": { "words": "anchors",,,, } }
  QHash<QString, QHash<QString, QString>> cache;
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

void SearchDb::searchByAppName(const QString& app_name,
                               const QString& keyword,
                               SearchResultList& result) {
  const QHash<QString, QString>& app_dict = p_->cache[app_name];
  for (const QString& words : app_dict.keys()) {
    if (result.size() >= kResultLimitation) {
      break;
    }

    if (words.indexOf(keyword) > -1) {
      result.append({
                        app_name,
                        app_dict.value(words)
                    });
    }
  }
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

  // Prepare search cache
  if (!query.exec(kSelectAll)) {
    qCritical() << "Failed to select search items"
                << query.lastError().text();
    return;
  }

  while (query.next()) {
    const QString app_name = query.value(1).toString();
    if (!p_->cache.contains(app_name)) {
      p_->cache.insert(app_name, QHash<QString, QString>());
    }
    p_->cache[app_name].insert(query.value(4).toString(),
                               query.value(2).toString());
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
  QHash<QString, QString> anchor_dict;
  for (int i = 0; ok && (i < anchors.length()); ++i) {
    const std::string content_std(contents.at(i).toLower().toStdString());
    std::vector<std::string> word_list;
    p_->jieba->CutForSearch(content_std, word_list);
    const std::string words_std = limonp::Join(word_list.begin(),
                                               word_list.end(),
                                               "/");
    const QString words = QString::fromStdString(words_std);
    // Add to memory cache.
    anchor_dict.insert(words, anchors.at(i));

    // Save to database.
    query.bindValue(0, app_name);
    query.bindValue(1, anchors.at(i));
    query.bindValue(2, contents.at(i));
    query.bindValue(3, words);
    ok = query.exec();
  }
  p_->cache.insert(app_name, anchor_dict);

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

  if (app_name.isEmpty()) {
    // Global search
    for (const QString& name : p_->cache.keys()) {
      if (result.size() >= kResultLimitation) {
        break;
      }
      this->searchByAppName(name, keyword, result);
    }
  } else if (p_->cache.contains(app_name)) {
    // Search in app_name.
    this->searchByAppName(app_name, keyword, result);
  }

  qDebug() << "result size:" << result.size() << keyword << app_name;

  emit this->searchResult(app_name, keyword, result);
}

}  // namespace dman