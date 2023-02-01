// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEEPIN_MANUAL_CONTROLLER_SEARCH_MANAGER_H
#define DEEPIN_MANUAL_CONTROLLER_SEARCH_MANAGER_H

#include "controller/search_result.h"

#include <QObject>

class QThread;
class SearchDb;

class SearchManager : public QObject
{
    Q_OBJECT

public:
    explicit SearchManager(QObject *parent = nullptr);

    ~SearchManager() override;


signals:
    void searchAnchor(const QString &keyword);
    void searchAnchorResult(const QString &keyword,
                            const SearchAnchorResultList &result);

    void searchContent(const QString &keyword);
    void searchContentResult(const QString &app_name,
                             const QStringList &anchors,
                             const QStringList &anchorIdList,
                             const QStringList &contents);
    void searchContentMismatch(const QString &keyword);
    void updateModule();
    void updateDb();

    void updateDb();

private:
    void initSearchManager();

    SearchDb *db_;
    QThread *db_thread_;
};

#endif // DEEPIN_MANUAL_CONTROLLER_SEARCH_MANAGER_H
