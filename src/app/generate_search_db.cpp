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

#include "base/command.h"
#include "controller/search_db.h"
#include "dpinyin.h"

int main(int argc, char **argv)
{
    qputenv("LANG", "en_US.UTF8");
    qputenv("LC_ALL", "en_US.UTF8");
    qputenv("LANGUAGE", "en_US");

    QCoreApplication app(argc, argv);

    //设置当前工作目录为 manual/src/web
    QDir::setCurrent(DMAN_SEARCH_WORK_DIR);

//    //获取manual/manual-assets目录下的文件夹列表
//    QStringList list = QDir(DMAN_ORIG_MANUAL_DIR).entryList(QDir::NoDotAndDotDot | QDir::Dirs);

    //遍历manual/manual-assets目录下的文件夹列表
    for (QString &dbType :
            QDir(DMAN_ORIG_MANUAL_DIR).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
        QString strCreateDbPath = DMAN_SEARCH_CREATE_DB_PATH;
        strCreateDbPath += "/" + dbType;

        QDir dir(strCreateDbPath);
        if (!dir.exists()) {
            dir.mkpath(strCreateDbPath);
        }
        strCreateDbPath += "/search.db";

//        dman::SearchDb db;
        SearchDb db;
        db.initDb(strCreateDbPath);
        db.initSearchTable();

        QStringList list = {"zh_CN","en_US"};
//        list << "zh_CN"
//             << "en_US";
        for (QString &locale : list) {
            QString strManualDir = DMAN_ORIG_MANUAL_DIR;
            strManualDir += "/" + dbType;
            for (QString &app_name :
                    QDir(strManualDir).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                const QString md_file = QStringList {strManualDir, app_name, locale, "index.md"}.join(QDir::separator());
                if (!QFileInfo(md_file).isFile()) {
                    qWarning() << md_file << "not found";
                    continue;
                }

                QDir manualDir = QDir(DMAN_ORIG_MANUAL_DIR);
                manualDir.cdUp();
                qDebug() << manualDir.path();
                QString searchIndexFilePath = QString("%1/%2/%3/%4")
                                              .arg(manualDir.path(), "src", "web", "toSearchIndex.js");
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

                qDebug()<<"====>"<<out;
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
                    QString title_us = anchor.at(1).toString();
                    anchors.append(title_ch);
                    if (locale == "zh_CN") {
                        QString str = Dtk::Core::Chinese2Pinyin(title_ch).remove(QRegExp("[1-9]"));
                        anchorSpellList.append(str);
                        if (id == "h0") {
                            QString anchorInitial;
                            for (int i = 0; i < title_ch.length(); i++) {
                                anchorInitial.append(Dtk::Core::Chinese2Pinyin(title_ch.at(i)).left(1));
                            }
                            anchorInitialList.append(anchorInitial);
                        } else {
                            anchorInitialList.append("");
                        }
                    } else if (locale == "en_US") {
                        if (id == "h0") {
                            QStringList listTitle = title_us.split(" ");
                            QString anchorInitial;
                            for (QString str : listTitle) {
                                anchorInitial.append(str.left(1));
                            }
                            anchorInitialList.append(anchorInitial);
                        } else {
                            anchorInitialList.append("");
                        }
                        anchorSpellList.append(title_us.remove(" "));
                    }
                    const QString content = anchor.at(2).toString();
                    contents.append(content);
                }

                if (!invalid_entry) {
                    qDebug() << "add search entry" << app_name << locale << anchors << endl;
                    db.addSearchEntry(dbType, app_name, locale, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
                }
            }
        }
    }
}
