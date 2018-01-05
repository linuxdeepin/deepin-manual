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

#ifndef DEEPIN_MANUAL_VIEW_SEARCH_PROXY_H
#define DEEPIN_MANUAL_VIEW_SEARCH_PROXY_H

#include <QObject>

namespace dman {

class SearchManager;

class SearchProxy : public QObject {
  Q_OBJECT
 public:
  explicit SearchProxy(SearchManager* manager, QObject* parent = nullptr);
  ~SearchProxy() override;

 signals:
  void mismatch(const QString& keyword);
  void match(const QString& anchor);
  void globalMatch(const QString& app_name, const QString& anchor);

 public slots:
  void setCurrentApp(const QString& app_name);

  void addSearchEntry(const QString& app_name,
                      const QStringList& anchors,
                      const QStringList& contents);

 private:
  SearchManager* manager_ = nullptr;
};

}  // namespace dman

#endif  // DEEPIN_MANUAL_VIEW_SEARCH_PROXY_H
