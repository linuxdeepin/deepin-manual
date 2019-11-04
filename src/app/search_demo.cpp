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

#include <QCoreApplication>
#include <QDebug>

#include "controller/search_manager.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    dman::SearchManager manager;
    QObject::connect(&manager, &dman::SearchManager::searchAnchorResult,
                     [](const QString & keyword,
    const dman::SearchAnchorResultList & result) {
        qDebug() << keyword << result.size();
        for (const dman::SearchAnchorResult &item : result) {
            qDebug() << item.anchor << item.app_name;
        }
    });

    QObject::connect(&manager, &dman::SearchManager::searchContentResult,
                     [](const QString & app_name,
                        const QStringList & anchors,
                        const QStringList & contents) {
        qDebug() << app_name;
        for (int i = 0; i < anchors.size(); ++i) {
            qDebug() << anchors.at(i) << contents.at(i);
        }
    });
    manager.searchAnchor("application");
    manager.searchContent("application");

    return app.exec();
}
