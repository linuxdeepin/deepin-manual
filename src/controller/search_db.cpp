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
    "lang TEXT,"
    "anchor TEXT,"
    "content TEXT,"
    "words TEXT)";

const char kIndexSchema[] = "CREATE INDEX IF NOT EXISTS search_idx "
    "ON search (id, appName, lang)";

const char kDeleteEntryByApp[] = "DELETE FROM search WHERE appName = ?";
const char kInsertEntry[] = "INSERT INTO search "
    "(appName, lang, anchor, content, words) "
    "VALUES (?, ?, ?, ?, ?)";

const char kSelectAll[] = "SELECT * FROM search";
const char kSelectAnchor[] = "SELECT appName, anchor FROM search "
    "WHERE lang = ':lang' AND "
    "anchor LIKE '%:anchor%' --case insensitive";
const char kSelectContent[] = "SELECT appName, anchor, content FROM search "
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
}

void SearchDb::handleAddSearchEntry(const QString& app_name,
                                    const QString& lang,
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

    // Save to database.
    query.bindValue(0, app_name);
    query.bindValue(1, lang);
    query.bindValue(2, anchors.at(i));
    query.bindValue(3, contents.at(i));
    query.bindValue(4, words);
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
  const QString sql = QString(kSelectAnchor)
      .replace(":anchor", keyword)
      .replace(":lang", lang);
  if (query.exec(sql)) {
    while (query.next() && (result.size() < kResultLimitation)) {
      result.append(SearchAnchorResult{
          query.value(0).toString(),
          query.value(1).toString()
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
  const QString sql = QString(kSelectContent)
      .replace(":lang", lang)
      .replace(":content", keyword);
  if (query.exec(sql)) {
    QString last_app_name;
    QStringList anchors;
    QStringList contents;
    while (query.next()) {
      const QString app_name = query.value(0).toString();
      const QString anchor = query.value(1).toString();
      const QString content = query.value(2).toString();
      if ((app_name == last_app_name) || last_app_name.isEmpty()) {
        anchors.append(anchor);
        contents.append(content);
      } else {
        if (!anchors.isEmpty()) {
          emit this->searchContentResult(last_app_name, anchors, contents);
        }

        anchors.clear();
        contents.clear();
        last_app_name = app_name;
        anchors.append(anchor);
        contents.append(content);
      }
    }
  } else {
    qCritical() << "Failed to select contents:"
                << query.lastError().text();
  }
}

}  // namespace dman