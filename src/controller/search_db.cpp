/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
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
#include <DLog>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace dman {

namespace {

const char kSearchDropTable[] = "DROP TABLE IF EXISTS search";

const char kSearchTableSchema[] = "CREATE TABLE IF NOT EXISTS search "
                                  "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                  "appName TEXT,"
                                  "lang TEXT,"
                                  "anchor TEXT,"
                                  "anchorId TEXT,"
                                  "content TEXT)";

const char kSearchIndexSchema[] = "CREATE INDEX IF NOT EXISTS search_idx "
                                  "ON search (id, appName, lang)";

const char kSearchDeleteEntryByApp[] = "DELETE FROM search "
                                       "WHERE appName = ? AND lang = ?";
const char kSearchInsertEntry[] = "INSERT INTO search "
                                  "(appName, lang, anchor, anchorId, content) "
                                  "VALUES (?, ?, ?, ?, ?)";

//const char kSearchSelectAll[] = "SELECT * FROM search";

const char kSearchSelectAnchor[] = "SELECT t1.appName, t2.anchor appDisplayName, t1.anchor, t1.anchorId FROM search t1 "
                                   "LEFT JOIN (SELECT anchor,appName FROM search where anchorId='h0' and lang=':lang') t2 "
                                   "WHERE t1.appName=t2.appName "
                                   "AND t1.lang = ':lang' "
                                   "AND t1.anchor LIKE '%:anchor%' --case insensitive";

const char kSearchSelectContent[] = "SELECT appName, anchor, anchorId, content "
                                    "FROM search "
                                    "WHERE lang = ':lang' AND "
                                    "content LIKE '%:content%' --case insensitive";

const int kResultLimitation = INT_MAX;

}  // namespace

struct SearchDbPrivate {
    QSqlDatabase db;
};

SearchDb::SearchDb(QObject *parent)
    : QObject(parent),
      p_(new SearchDbPrivate())
{
    qRegisterMetaType<SearchAnchorResult>("SearchAnchorResult");
    qRegisterMetaType<SearchAnchorResultList>("SearchAnchorResultList");
    qRegisterMetaType<SearchContentResult>("SearchContentResult");
    qRegisterMetaType<SearchContentResultList>("SearchContentResultList");

    this->initConnections();
}

SearchDb::~SearchDb()
{
    if (p_ != nullptr) {
        if (p_->db.isOpen()) {
            p_->db.close();
        }
        delete p_;
        p_ = nullptr;
    }
}

void SearchDb::initConnections()
{
    connect(this, &SearchDb::initDbAsync,
            this, &SearchDb::initDb);
    connect(this, &SearchDb::searchAnchor,
            this, &SearchDb::handleSearchAnchor);
    connect(this, &SearchDb::searchContent,
            this, &SearchDb::handleSearchContent);
}

void SearchDb::initDb(const QString &db_path)
{
    qDebug() << "initDb database path is:" << db_path << endl;
    p_->db = QSqlDatabase::addDatabase("QSQLITE");
    p_->db.setDatabaseName(db_path);
    if (!p_->db.open()) {
        qCritical() << "Failed to open search db:" << db_path;
        return;
    }
}

void SearchDb::initSearchTable()
{
    QSqlQuery query(p_->db);
    if (!query.exec(kSearchDropTable)) {
        qCritical() << "Failed to drop search table";
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

void SearchDb::addSearchEntry(const QString &app_name,
                              const QString &lang,
                              const QStringList &anchors,
                              const QStringList &anchorIdList,
                              const QStringList &contents)
{
    Q_ASSERT(p_->db.isOpen());
    Q_ASSERT(anchors.length() == contents.length());
    qDebug() << "addSearchEntry()" << app_name << lang << anchors << contents;

    QStringList newContents = contents;
    for(int i=0; i<contents.size(); i++) {
        QString content = contents.at(i);
        content = content.replace("icon/", "/usr/share/deepin-manual/manual/" + app_name + "/" + lang + "/icon/");
        newContents.replace(i, content);
    }

    if (anchors.length() != newContents.length() ||
            anchors.length() != anchorIdList.length()) {
        qCritical() << "anchor list and contents mismatch:"
                    << anchors.length() << newContents.length();
        return;
    }

    QSqlQuery query(p_->db);
    query.prepare(kSearchDeleteEntryByApp);
    query.bindValue(0, app_name);
    query.bindValue(1, lang);
    if (!query.exec()) {
        qCritical() << "Failed to delete search entry:"
                    << query.lastError().text();
        return;
    }

    if (!p_->db.transaction()) {
        qWarning() << "Failed to start db transaction!";
        return;
    }
    query.prepare(kSearchInsertEntry);
    QStringList app_names;
    QStringList lang_list;
    for (int i = 0; i < anchors.length(); ++i) {
        app_names.append(app_name);
        lang_list.append(lang);
    }
    query.bindValue(0, app_names);
    query.bindValue(1, lang_list);
    query.bindValue(2, anchors);
    query.bindValue(3, anchorIdList);
    query.bindValue(4, newContents);
    bool ok = query.execBatch();

    if (!ok) {
        p_->db.rollback();
        qCritical() << "Failed to insert search entry:"
                    << query.lastError().text();
    } else {
        p_->db.commit();
    }
}

void SearchDb::handleSearchAnchor(const QString &keyword)
{
    qDebug() << Q_FUNC_INFO << keyword;
    Q_ASSERT(p_->db.isOpen());

    SearchAnchorResultList result;

    QSqlQuery query(p_->db);
    const QString lang = QLocale().name();
    const QString sql = QString(kSearchSelectAnchor)
                        .replace(":anchor", keyword)
                        .replace(":lang", lang);
    qDebug() << "handleSearchAnchor sql is:" << sql;
    if (query.exec(sql)) {
        while (query.next() && (result.size() < kResultLimitation)) {
            result.append(SearchAnchorResult{
                query.value(0).toString(),
                query.value(1).toString(),
                query.value(2).toString(),
                query.value(3).toString(),
            });
        }
    } else {
        qCritical() << "Failed to select anchor:"
                    << query.lastError().text();
    }

    qDebug() << "result size:" << result.size() << keyword;

    emit this->searchAnchorResult(keyword, result);
}

void SearchDb::handleSearchContent(const QString &keyword)
{
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
            result_empty = false;
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

        // Last record.
        if (!result_empty) {
            emit this->searchContentResult(last_app_name, anchors,
                                           anchorIds, contents);
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
