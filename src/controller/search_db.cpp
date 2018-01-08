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
    "content TEXT)";

const char kIndexSchema[] = "CREATE INDEX IF NOT EXISTS search_idx "
    "ON search "
    "(id,"
    "appName)";

const char kDeleteEntryByApp[] = "DELETE FROM search WHERE appName = ?";
const char kInsertEntry[] = "INSERT INTO search(appName, anchor, content) "
    "VALUES (?, ?, ?)";

const char kSelectAll[] = "SELECT * FROM search";
const char kSelectApp[] = "SELECT * FROm search WHERE appName = ?";

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

bool MatchKeyword(cppjieba::Jieba* jieba,
                  const QString& content,
                  const QString& keyword) {
//  qDebug() << Q_FUNC_INFO << content << keyword;
  const std::string content_std(content.toLower().toStdString());
  const std::string keyword_std(keyword.toLower().toStdString());
  std::vector<std::string> words;
  jieba->CutForSearch(content_std, words);
  for (const std::string& word : words) {
    if (word == keyword_std) {
      return true;
    }
  }

  return false;
}

}  // namespace

struct SearchDbPrivate {
  QSqlDatabase db;
  cppjieba::Jieba* jieba = nullptr;
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
    query.bindValue(0, app_name);
    query.bindValue(1, anchors.at(i));
    query.bindValue(2, contents.at(i));
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
      const QString content = query.value(3).toString();
      if (MatchKeyword(p_->jieba, content, keyword)) {
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

  emit this->searchResult(app_name, keyword, result);
}

}  // namespace dman