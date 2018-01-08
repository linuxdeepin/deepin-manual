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

#ifndef DEEPIN_MANUAL_CONTROLLER_SEARCH_MANAGER_H
#define DEEPIN_MANUAL_CONTROLLER_SEARCH_MANAGER_H

#include <QObject>
#include <QList>

#include "controller/search_result.h"

class QThread;

namespace dman {

class SearchDb;

class SearchManager : public QObject {
 Q_OBJECT

 public:
  explicit SearchManager(QObject* parent = nullptr);

  ~SearchManager() override;

 signals:
  void addSearchEntry(const QString& app_name,
                      const QStringList& anchors,
                      const QStringList& contents);
  void search(const QString& app_name, const QString& keyword);
  void searchResult(const QString& app_name,
                    const QString& keyword,
                    const SearchResultList& result);

 private:
  SearchDb* db_ = nullptr;
  QThread* db_thread_ = nullptr;
};

}  // namespace dman

#endif  // DEEPIN_MANUAL_CONTROLLER_SEARCH_MANAGER_H
