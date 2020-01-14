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
#include <QDebug>
#include <QDir>
#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "base/command.h"
#include "controller/search_db.h"

int main(int argc, char **argv)
{
    qputenv("LANG", "en_US.UTF8");
    qputenv("LC_ALL", "en_US.UTF8");
    qputenv("LANGUAGE", "en_US");

    QCoreApplication app(argc, argv);

    QDir::setCurrent(DMAN_SEARCH_WORK_DIR);

    // hash--> key: old name   value: new name
    const QHash<QString, QString> hash = {
        {"", "downloader"},                                   //下载器  ***
        {"", "deepin-voice-recorder"},                        //录音 ***
        {"deepin-voice-note", "deepin-voice-note"},           //语音记事本
        {"deepin-terminal", "deepin-terminal"},               //终端
        {"deepin-system-monitor", "deepin-system-monitor"},   //系统监视器
        {"", "deepin-screenshot"},                            //截屏
        {"deepin-screen-capture", "deepin-screen-recorder"},  //录屏
        {"deepin-repair", "deepin-repair-tools"},             //系统修复工具
        {"", "deepin-remote-assistance"},                     //远程协助**
        {"", "deepin-reader"},                                //文档查看器**
        {"", "deepin-presentation-assistant"},                //演示助手**
        {"deepin-picker", "deepin-picker"},                   //取色器
        {"deepin-music", "deepin-music"},                     //音乐
        {"deepin-movie", "deepin-movie"},                     //影院
        {"", "deepin-log-viewer"},                            //日志收集工具**
        {"deepin-image-viewer", "deepin-image-viewer"},       //看图
        {"deepin-graphics-driver-manager", "deepin-graphics-driver-manager"},  //显卡驱动管理器
        {"deepin-font-manager", "deepin-font-manager"},                        //字体管理器
        {"deepin-editor", "deepin-editor"},                                    //编辑器
        {"deepin-draw", "deepin-draw"},                                        //画板
        {"", "deepin-document-viewer"},                                        //文档查看器**
        {"", "deepin-devicemanager"},                                          //设备管理器**
        {"deepin-package-manager", "deepin-deb-installer"},                    //软件包安装器
        {"deepin-contact", "deepin-contacts"},                                 //联系人
        {"deepin-compressor", "deepin-compressor"},                            //归档管理器
        {"", "deepin-cloud-scan"},                                             //云扫描**
        {"", "deepin-cloud-print"},                                            //云打印**
        {"deepin-clone", "deepin-clone"},                                      //备份还原工具
        {"deepin-calculator", "deepin-calculator"},                            //计算器
        {"", "deepin-browser"},                                                //浏览器**
        {"deepin-boot-maker", "deepin-boot-maker"},   //启动盘制作工具
        {"deepin-appstore", "deepin-app-store"},      //应用商店
        {"deepin-album", "deepin-album"},             //相册
        {"deepin-print management", "dde-printer"},   //打印管理器 **
        {"deepin-file-manager", "dde-file-manager"},  //文件管理器**
        {"deepin-calendar", "dde-calendar"},          //日历
        {"dde", "dde"},                               //桌面环境
        {"", "chineseime"}                            //中文输入法**
    };

    QDir dirDir("/home/archermind/Documents/gitWork/manual-temp/server/dde");
    bool b = dirDir.remove("/home/archermind/Documents/gitWork/manual-temp");
    qDebug() << b;
    return 1;

    QString strOldPath = DMAN_ORIG_MANUAL_DIR;
    strOldPath += "-temp";
    for (const QString &strType : QDir(strOldPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
        QString strDirPath = strOldPath + "/" + strType;
        QDir dirDir(strDirPath);
        qDebug() << "strDirPath";

        for (const QString &strFile :
             QDir(strDirPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
            QHashIterator<QString, QString> iterator(hash);

            while (iterator.hasNext()) {
                iterator.next();
                if (!iterator.key().isEmpty()) {
                    if (strFile.contains(iterator.key())) {
                        bool b = dirDir.rename(strFile, iterator.value());
                        qDebug() << b << " ===  old:" << strFile << "  new:" << iterator.value();
                        break;
                    }
                } else {
                    continue;
                }
            }
        }
    }

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
                    const QString title = anchor.at(1).toString();
                    anchors.append(title);
                    const QString content = anchor.at(2).toString();
                    contents.append(content);
                }

                if (!invalid_entry) {
                    qDebug() << "add search entry" << app_name << locale << anchors << endl;
                    db.addSearchEntry(app_name, locale, anchors, anchorIdList, contents);
                }
            }
        }
    }
}
