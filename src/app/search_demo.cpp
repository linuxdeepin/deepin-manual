// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QCoreApplication>
#include <DLog>

#include "controller/search_manager.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    qDebug() << "QCoreApplication initialized";

    qDebug() << "Creating SearchManager instance";
    dman::SearchManager manager;
    qDebug() << "SearchManager initialized";
    QObject::connect(&manager, &dman::SearchManager::searchAnchorResult,
                     [](const QString &keyword,
                        const dman::SearchAnchorResultList &result) {
                         qDebug() << keyword << result.size();
                         for (const dman::SearchAnchorResult &item : result) {
                             qDebug() << item.anchor << item.app_name;
                         }
                     });

    QObject::connect(&manager, &dman::SearchManager::searchContentResult,
                     [](const QString &app_name,
                        const QStringList &anchors,
                        const QStringList &contents) {
                         qDebug() << app_name;
                         for (int i = 0; i < anchors.size(); ++i) {
                             qDebug() << anchors.at(i) << contents.at(i);
                         }
                     });
    qDebug() << "Starting anchor search for keyword: application";
    manager.searchAnchor("application");
    manager.searchContent("application");

    return app.exec();
}
