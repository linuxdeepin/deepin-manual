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

#ifndef DEEPIN_MANUAL_CONTROLLER_SEARCH_DB_H
#define DEEPIN_MANUAL_CONTROLLER_SEARCH_DB_H

#include <QObject>

#include "controller/search_result.h"

namespace dman {

struct SearchDbPrivate;

class SearchDb : public QObject {
  Q_OBJECT
 public:
  explicit SearchDb(QObject* parent = nullptr);
  ~SearchDb() override;

 signals:
  void initDb();
  void addSearchEntry(const QString& app_name,
                      const QString& lang,
                      const QStringList& anchors,
                      const QStringList& contents);

  // Only search in anchor.
  void searchAnchor(const QString& keyword);
  void searchAnchorResult(const QString& keyword,
                          const SearchAnchorResultList& result);

  void searchContent(const QString& keyword);
  void searchContentResult(const QString& keyword,
                           const SearchContentResultList& result);

 private:
  void initConnections();
  void searchByAppName(const QString& app_name,
                       const QString& keyword,
                       SearchAnchorResultList& result);

  SearchDbPrivate* p_ = nullptr;

 private slots:
  void handleInitDb();
  void handleAddSearchEntry(const QString& app_name,
                            const QString& lang,
                            const QStringList& anchors,
                            const QStringList& contents);
  void handleSearchAnchor(const QString& keyword);
  void handleSearchContent(const QString& keyword);
};

}  // namespace dman

#endif  // DEEPIN_MANUAL_CONTROLLER_SEARCH_DB_H
