// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEEPIN_MANUAL_VIEW_SEARCH_PROXY_H
#define DEEPIN_MANUAL_VIEW_SEARCH_PROXY_H

#include <QObject>

class SearchProxy : public QObject
{
    Q_OBJECT
public:
    explicit SearchProxy(QObject *parent = nullptr);
    ~SearchProxy() override;

signals:
    void mismatch(const QString &keyword);
    void onContentResult(const QString &app_name,
                         const QStringList &anchors,
                         const QStringList &anchorIdList,
                         const QStringList &contents);
    void reloadPage(const QStringList &appList);
    void setKeyword(const QString &keyword);
    void updateSearchResult();

public slots:
    void getKeyword(const QString &keyword);
    void updateSearch(const QString &keyword);
};

#endif // DEEPIN_MANUAL_VIEW_SEARCH_PROXY_H
