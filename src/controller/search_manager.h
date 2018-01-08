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

class QThread;

namespace dman {

class SearchDb;

class SearchManager : public QObject {
 Q_OBJECT

 public:
  explicit SearchManager(QObject* parent = nullptr);

  ~SearchManager() override;

  struct Entry {
    QString app_name;
    QString anchor;
  };
  typedef QList<Entry> EntryList;

 signals:
  void mismatch(const QString& keyword);
  void globalMismatch(const QString& keyword);
//  void match(const QString& anchor);
//  void globalMatch(const QString& app_name, const QString& anchor);

 public slots:
  void search(const QString& keyword);

  void setCurrentApp(const QString& app_name);

  void addSearchEntry(const QString& app_name,
                      const QStringList& anchors,
                      const QStringList& contents);

 private:
  SearchDb* db_ = nullptr;
  QThread* db_thread_ = nullptr;
  QString current_app_;
};

}  // namespace dman

#endif  // DEEPIN_MANUAL_CONTROLLER_SEARCH_MANAGER_H
