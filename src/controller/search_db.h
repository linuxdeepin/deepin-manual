// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEEPIN_MANUAL_CONTROLLER_SEARCH_DB_H
#define DEEPIN_MANUAL_CONTROLLER_SEARCH_DB_H

#include "controller/search_result.h"

#include <QObject>

struct SearchDbPrivate;
struct searchStrct;

class SearchDb : public QObject
{
    Q_OBJECT
public:
    explicit SearchDb(QObject *parent = nullptr);
    ~SearchDb() override;

signals:
    void initDbAsync();
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

public slots:
    void initDb();
    /***************search表***************/
    void initSearchTable();
    void addSearchEntry(const QString &app_name,
                        const QString &lang,
                        const QStringList &anchors,
                        const QStringList &anchorInitialList,
                        const QStringList &anchorSpellList,
                        const QStringList &anchorIdList,
                        const QStringList &contents,
                        const QString &mdPath = "");
    void deleteSearchInfo(const QStringList &appName, const QStringList &lang);

    /***************filetime表***************/
    void initTimeTable();
    //文件信息插入数据库 （先删除数据，再插入数据）
    void insertFilesTimeEntry(const QStringList &listMdPath,
                              const QStringList &listDataTime);

    //根据mdPath删除表数据
    void deleteFilesTimeEntry(const QStringList &listMdPath);

    //查找filetime表所有内容, key:md文件路径 value: md文件更新时间
    QMap<QString, QString> selectAllFileTime();

    void getAllApp();

private:
    void initConnections();
    QString highlightKeyword(QString srcString, QString keyword);

    void sortSearchList(const QString &appName, const QStringList &anchors, const QStringList &anchorIds, const QStringList &contents, bool bIsTitleHigh);

    void omitHighlight(QString &highLight, const QString &keyword);

    QString insertHighlight(QString srcString, QString keyword);

    SearchDbPrivate *p_ = nullptr;
    QStringList strlistApp;
    QList<searchStrct> listStruct;
    int nH0OfList = 0;

private slots:
    void handleSearchAnchor(const QString &keyword);
    void handleSearchContent(const QString &keyword);
};

#endif // DEEPIN_MANUAL_CONTROLLER_SEARCH_DB_H
