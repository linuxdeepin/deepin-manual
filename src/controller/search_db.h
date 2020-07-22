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

#ifndef DEEPIN_MANUAL_CONTROLLER_SEARCH_DB_H
#define DEEPIN_MANUAL_CONTROLLER_SEARCH_DB_H

#include <QObject>

#include "controller/search_result.h"

namespace dman {

struct SearchDbPrivate;

class SearchDb : public QObject
{
    Q_OBJECT
public:
    explicit SearchDb(QObject *parent = nullptr);
    ~SearchDb() override;

signals:
    void initDbAsync(const QString &db_path);

    // Only search in anchor.
    void searchAnchor(const QString &keyword);
    void searchAnchorResult(const QString &keyword,
                            const SearchAnchorResultList &result);

    void searchContent(const QString &keyword);
    void searchContentResult(const QString &app_name,
                             const QStringList &anchors,
                             const QStringList &anchorIdList,
                             const QStringList &contents);
    void searchContentMismatch(const QString &keyword);
    void installApps(const QStringList &strlistApps);

public slots:
    void initDb(const QString &db_path);
    void initSearchTable();
    void addSearchEntry(const QString &system,
                        const QString &app_name,
                        const QString &lang,
                        const QStringList &anchors,
                        const QStringList &anchorInitialList,
                        const QStringList &anchorSpellList,
                        const QStringList &anchorIdList,
                        const QStringList &contents
                       );

private:
    void initConnections();
    inline QString highlightKeyword(QString srcString, QString keyword);

    SearchDbPrivate *p_ = nullptr;
    QStringList strlistApp;

private slots:
    void handleSearchAnchor(const QString &keyword);
    void handleSearchContent(const QString &keyword);
    void handleInstallApps(const QStringList &strlistApps);
};

}  // namespace dman

#endif  // DEEPIN_MANUAL_CONTROLLER_SEARCH_DB_H
