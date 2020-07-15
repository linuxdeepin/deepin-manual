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

/**
 * Generate search entry and save into sqlite database.
 * Call this program while build deb packages.
 */

#include <DLog>
#include <DSysInfo>
#include <QCoreApplication>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "base/command.h"
#include "controller/search_db.h"
#include "dpinyin.h"

int main(int argc, char **argv)
{
    qputenv("LANG", "en_US.UTF8");
    qputenv("LC_ALL", "en_US.UTF8");
    qputenv("LANGUAGE", "en_US");

    QCoreApplication app(argc, argv);

    QDir::setCurrent(DMAN_SEARCH_WORK_DIR);

    for (const QString &dbType :
            QDir(DMAN_ORIG_MANUAL_DIR).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
        QString strCreateDbPath = DMAN_SEARCH_CREATE_DB_PATH;
        strCreateDbPath += "/" + dbType;

        QDir dir(strCreateDbPath);
        if (!dir.exists()) {
            dir.mkpath(strCreateDbPath);
        }
        strCreateDbPath += "/search.db";

        dman::SearchDb db;
        db.initDb(strCreateDbPath);
        db.initSearchTable();

        for (const QString &locale : {"zh_CN", "en_US"}) {
            QString strManualDir = DMAN_ORIG_MANUAL_DIR;
            strManualDir += "/" + dbType;
            for (const QString &app_name :
                    QDir(strManualDir).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                const QString md_file =
                    QStringList {strManualDir, app_name, locale, "index.md"}.join(
                        QDir::separator());
                if (!QFileInfo(md_file).isFile()) {
                    qWarning() << md_file << "not found";
                    continue;
                }

                QDir manualDir = QDir(DMAN_ORIG_MANUAL_DIR);
                manualDir.cdUp();
                qDebug() << manualDir.path();
                QString searchIndexFilePath = QString("%1/%2/%3/%4")
                                              .arg(manualDir.path())
                                              .arg("src")
                                              .arg("web")
                                              .arg("toSearchIndex.js");
                qDebug() << searchIndexFilePath;
                QString out, err;
                //            QStringList cmdList = {"node"};
                //            const bool queryNode = dman::SpawnCmd("which", cmdList);
                //            if (!queryNode) {
                //                qWarning() << err << queryNode << md_file;
                //            }

                const QStringList cmd = {searchIndexFilePath, "-f", md_file};
                const bool ok = dman::SpawnCmd("/usr/bin/node", cmd, out, err);
                if (!ok) {
                    qWarning() << err << ok << md_file;
                    continue;
                }

                // Parse json
                QJsonDocument document = QJsonDocument::fromJson(out.toLocal8Bit());
                if (!document.isArray()) {
                    qWarning() << md_file << "Invalid json object:" << out;
                    continue;
                }

                const QJsonArray array = document.array();
                QStringList anchors;
                QStringList anchorIdList;
                QStringList anchorInitialList;
                QStringList anchorSpellList;
                QStringList contents;
                bool invalid_entry = false;
                for (const QJsonValue &value : array) {
                    if (!value.isArray()) {
                        invalid_entry = true;
                        qWarning() << md_file << "is not an array:" << value;
                        break;
                    }

                    const QJsonArray anchor = value.toArray();
                    const QString id = anchor.at(0).toString();
                    anchorIdList.append(id);
                    const QString title_ch = anchor.at(1).toString();
                    const QString title_us = anchor.at(1).toString();
                    anchors.append(title_ch);
                    if (locale == "zh_CN") {
                        QString anchorInitial;
                        for (int i = 0; i < title_ch.length(); i++) {
                            anchorInitial.append(Dtk::Core::Chinese2Pinyin(title_ch.at(i)).left(1));
                        }
                        anchorInitialList.append(anchorInitial);
                        if (id == "h0") {
                            QString str = Dtk::Core::Chinese2Pinyin(title_ch).remove(QRegExp("[1-9]"));
                            anchorSpellList.append(str);
                        } else {
                            anchorSpellList.append("null");
                        }

                    } else if (locale == "en_US") {
                        QStringList list = title_us.split(" ");
                        QString anchorInitial;
                        for (QString str : list) {
                            anchorInitial.append(str.left(1));
                        }
                        anchorInitialList.append(anchorInitial);
                        anchorSpellList.append(anchorInitial);
                    }
                    const QString content = anchor.at(2).toString();
                    contents.append(content);
                }

                if (!invalid_entry) {
                    qDebug() << "add search entry" << app_name << locale << anchors  << endl;
                    db.addSearchEntry(app_name, locale, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
                }
            }
        }
    }
}
