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

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "base/consts.h"
#include "cppjieba/Jieba.hpp"

namespace dman {

namespace {

const char kAppTableSchema[] = "CREATE TABLE IF NOT EXISTS app "
    "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "appName TEXT,"
    "lang TEXT,"
    "updated INTEGER,"
    "timestamp DATETIME)";

const char kAppIndexSchema[] = "CREATE INDEX IF NOT EXISTS app_idx "
    "ON search (id, appName, lang)";

const char kAppSelect[] = "SELECT * FROM app WHERE appName = ? and lang = ?";

const char kAppInsert[] = "INSERT INTO app "
    "(appName, lang, updated, timestamp) VALUES (?, ?, ?, ?)";

const char kAppUpdateTimestamp[] = "UPDATE app "
    "SET updated = 1, timestamp = :timestamp "
    "WHERE appName = :appName "
    "AND lang = :lang "
    "AND timestamp != :timestamp";

const char kSearchTableSchema[] = "CREATE TABLE IF NOT EXISTS search "
    "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "appName TEXT,"
    "lang TEXT,"
    "anchor TEXT,"
    "anchorId TEXT,"
    "content TEXT,"
    "words TEXT)";

const char kSearchIndexSchema[] = "CREATE INDEX IF NOT EXISTS search_idx "
    "ON search (id, appName, lang)";

const char kSearchDeleteEntryByApp[] = "DELETE FROM search WHERE appName = ?";
const char kSearchInsertEntry[] = "INSERT INTO search "
    "(appName, lang, anchor, anchorId, content, words) "
    "VALUES (?, ?, ?, ?, ?, ?)";

const char kSearchSelectAll[] = "SELECT * FROM search";
const char kSearchSelectAnchor[] = "SELECT appName, anchor, anchorId "
    "FROM search "
    "WHERE lang = ':lang' AND "
    "anchor LIKE '%:anchor%' --case insensitive";
const char kSearchSelectContent[] = "SELECT appName, anchor, anchorId, content "
    "FROM search "
    "WHERE lang = ':lang' AND "
    "content LIKE '%:content%' --case insensitive";

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
  bool is_update_cache_emitted = false;
  cppjieba::Jieba* jieba = nullptr;
};

SearchDb::SearchDb(QObject* parent)
    : QObject(parent),
      p_(new SearchDbPrivate()) {
  qRegisterMetaType<SearchAnchorResult>("SearchAnchorResult");
  qRegisterMetaType<SearchAnchorResultList>("SearchAnchorResultList");
  qRegisterMetaType<SearchContentResult>("SearchContentResult");
  qRegisterMetaType<SearchContentResultList>("SearchContentResultList");

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
  connect(this, &SearchDb::updateSearchCache,
          this, &SearchDb::handleUpdateSearchCache);
  connect(this, &SearchDb::addSearchEntry,
          this, &SearchDb::handleAddSearchEntry);
  connect(this, &SearchDb::searchAnchor,
          this, &SearchDb::handleSearchAnchor);
  connect(this, &SearchDb::searchContent,
          this, &SearchDb::handleSearchContent);
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
  if (!query.exec(kAppTableSchema)) {
    qCritical() << "Failed to initialize app table:"
                << query.lastError().text();
    return;
  }
  if (!query.exec(kAppIndexSchema)) {
    qCritical() << "Failed to create index for app table"
                << query.lastError().text();
    return;
  }

  if (!query.exec(kSearchTableSchema)) {
    qCritical() << "Failed to initialize search table:"
                << query.lastError().text();
    return;
  }
  if (!query.exec(kSearchIndexSchema)) {
    qCritical() << "Failed to create index for search table"
                << query.lastError().text();
    return;
  }
}

void SearchDb::handleUpdateSearchCache(const QString& manual_dir,
                                       const QStringList& manuals) {
  qDebug() << Q_FUNC_INFO << manual_dir << manuals;
  // 1. Get app list which are available to update.
  // 2. Request update search entry.
  // 2.1. Discard other responses.
  // 3. Write search entry to db.

  // This slot can only be triggered once in current process.
  if (p_->is_update_cache_emitted) {
    return;
  }
  p_->is_update_cache_emitted = true;

  const QString locale = QLocale().name();
  QDir dir(manual_dir);
  QSqlQuery query(p_->db);

  for (const QString& app_name : manuals) {
    QDateTime mtime;
    const QString filepath = QStringList{
        manual_dir,
        app_name,
        locale,
        "index.md",
    }.join(QDir::separator());
    QFileInfo file_info(filepath);
    if (!file_info.isFile()) {
      qWarning() << "manual not found:" << filepath;
      continue;
    }

    mtime = file_info.lastModified();
    query.prepare(kAppSelect);
    query.bindValue(0, app_name);
    query.bindValue(1, locale);
    if (!query.exec()) {
      qWarning() << Q_FUNC_INFO << query.lastError().text();
      continue;
    }

    // 1. not found, insert
    // 2. exists, updated
    // 3. exists, need to update
    if (query.next()) {
      // Update record.
      qDebug() << "update:" << filepath;
      query.prepare(kAppUpdateTimestamp);
      query.bindValue(":timestamp", mtime.toMSecsSinceEpoch());
      query.bindValue(":appName", app_name);
      query.bindValue(":lang", locale);
    } else {
      // Insert record.
      query.prepare(kAppInsert);
      query.bindValue(0, app_name);
      query.bindValue(1, locale);
      query.bindValue(2, 1);
      query.bindValue(3, mtime.toMSecsSinceEpoch());
    }
    if (!query.exec()) {
      qCritical() << Q_FUNC_INFO << query.lastError().text();
    }
  }

  p_->db.commit();


}

void SearchDb::handleAddSearchEntry(const QString& app_name,
                                    const QString& lang,
                                    const QStringList& anchors,
                                    const QStringList& anchorIdList,
                                    const QStringList& contents) {
  Q_ASSERT(p_->db.isOpen());
  Q_ASSERT(anchors.length() == contents.length());
  qDebug() << Q_FUNC_INFO << app_name;

  // Do not insert into db if this search entry no need to update.
  return;

  if (anchors.length() != contents.length() ||
      anchors.length() != anchorIdList.length()) {
    qCritical() << "anchor list and contents mismatch:"
                << anchors.length() << contents.length();
    return;
  }

  QSqlQuery query(p_->db);
  query.prepare(kSearchDeleteEntryByApp);
  query.bindValue(0, app_name);
  if (!query.exec()) {
    qCritical() << "Failed to delete search entry:"
                << query.lastError().text();
    return;
  }

  query.prepare(kSearchInsertEntry);
  bool ok = true;
  for (int i = 0; ok && (i < anchors.length()); ++i) {
    // NOTE(Shaohua): Disable jieba
//    const std::string content_std(contents.at(i).toLower().toStdString());
//    std::vector<std::string> word_list;
//    p_->jieba->CutForSearch(content_std, word_list);
//    const std::string words_std = limonp::Join(word_list.begin(),
//                                               word_list.end(),
//                                               "/");
//    const QString words = QString::fromStdString(words_std);
    const QString words;

    // Save to database.
    query.bindValue(0, app_name);
    query.bindValue(1, lang);
    query.bindValue(2, anchors.at(i));
    query.bindValue(3, anchorIdList.at(i));
    query.bindValue(4, contents.at(i));
    query.bindValue(5, words);
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

void SearchDb::handleSearchAnchor(const QString& keyword) {
  qDebug() << Q_FUNC_INFO << keyword;
  Q_ASSERT(p_->db.isOpen());

  SearchAnchorResultList result;

  QSqlQuery query(p_->db);
  const QString lang = QLocale().name();
  const QString sql = QString(kSearchSelectAnchor)
      .replace(":anchor", keyword)
      .replace(":lang", lang);
  if (query.exec(sql)) {
    while (query.next() && (result.size() < kResultLimitation)) {
      result.append(SearchAnchorResult{
          query.value(0).toString(),
          query.value(1).toString(),
          query.value(2).toString(),
      });
    }
  } else {
    qCritical() << "Failed to select anchor:"
                << query.lastError().text();
  }

  qDebug() << "result size:" << result.size() << keyword;

  emit this->searchAnchorResult(keyword, result);
}

void SearchDb::handleSearchContent(const QString& keyword) {
  qDebug() << Q_FUNC_INFO << keyword;

  Q_ASSERT(p_->db.isOpen());

  QSqlQuery query(p_->db);
  const QString lang = QLocale().name();
  const QString sql = QString(kSearchSelectContent)
      .replace(":lang", lang)
      .replace(":content", keyword);
  bool result_empty = true;
  if (query.exec(sql)) {
    QString last_app_name;
    QStringList anchors;
    QStringList anchorIds;
    QStringList contents;
    while (query.next()) {
      const QString app_name = query.value(0).toString();
      const QString anchor = query.value(1).toString();
      const QString anchorId = query.value(2).toString();
      const QString content = query.value(3).toString();
      if (app_name == last_app_name) {
        anchors.append(anchor);
        anchorIds.append(anchorId);
        contents.append(content);
      } else {
        if (!last_app_name.isEmpty()) {
          result_empty = false;
          qDebug() << Q_FUNC_INFO << "emit searchContentResult()";
          emit this->searchContentResult(last_app_name, anchors,
                                         anchorIds, contents);
        }

        anchors.clear();
        anchorIds.clear();
        contents.clear();
        last_app_name = app_name;
        anchors.append(anchor);
        anchorIds.append(anchorId);
        contents.append(content);
      }
    }
  } else {
    qCritical() << "Failed to select contents:"
                << query.lastError().text();
  }

  if (result_empty) {
    emit this->searchContentMismatch(keyword);
  }
}

}  // namespace dman