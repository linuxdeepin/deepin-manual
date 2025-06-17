// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "filewatcher.h"
#include "base/utils.h"
#include "base/consts.h"
#include "base/ddlog.h"

#include <QDir>
#include <QDebug>
#include <DSysInfo>

fileWatcher::fileWatcher(QObject *parent)
    : QObject(parent)
    , watcherObj(new QFileSystemWatcher(this))
    , timerObj(new QTimer(this))
{
    qCDebug(app) << "Initializing file watcher";
    timerObj->setSingleShot(true);
    timerObj->setInterval(3 * 1000);
    connect(timerObj, &QTimer::timeout, this, &fileWatcher::onTimerOut);
    connect(watcherObj, &QFileSystemWatcher::fileChanged, this, &fileWatcher::onChangeFile);
    connect(watcherObj, &QFileSystemWatcher::directoryChanged, this, &fileWatcher::onChangeDirSlot);
    monitorFile();
    qCDebug(app) << "File watcher initialized successfully";
}

void fileWatcher::setFileMap(QMap<QString, QString> &map)
{
    mapOld = map;
}

/**
 * @brief helperManager::checkMap 白名单对比，得到差异列表信息
 * @param mapOld 旧的文件列表信息（key:文件路径 value:文件更新时间）
 * @param mapNow 当前文件列表信息
 * @param deleteList 比较得出删除的文件
 * @param addList 比较得出新增的文件（文件增加 & 文件被修改）
 * @param addTime 新增文件更新时间
 */
void fileWatcher::checkMap(QMap<QString, QString> &mapOld, QMap<QString, QString> &mapNow, QStringList &deleteList, QStringList &addList, QStringList &addTime)
{
    QList<QString> listOldMd = mapOld.keys();
    QList<QString> listNowMd = mapNow.keys();
    qCDebug(app) << "checkMap listNowMd:" << listNowMd;
    qCDebug(app) << "checkMap listOldMd:" << listOldMd;

    for (const QString &mdPath : listOldMd) {
        if (!listNowMd.contains(mdPath)) {
            deleteList.append(mdPath);
            qCDebug(app) << "Delete file:" << mdPath;
        }
    }

    for (const QString &mdPath : listNowMd) {
        if (!listOldMd.contains(mdPath)) {
            addList.append(mdPath);
            qCDebug(app) << "Add file:" << mdPath;
        } else if (mapOld.value(mdPath) != mapNow.value(mdPath)) {
            addList.append(mdPath);
            qCDebug(app) << "Modify file:" << mdPath;
        }
    }

    if (!addList.isEmpty()) {
        qCDebug(app) << "Add list:" << addList;
        for (const QString &file : addList) {
            addTime.append(mapNow.value(file));
        }
    }
    qCDebug(app) << "Delete list:" << deleteList;
}


/**
 * @brief fileWatcher::monitorFile 监控特定目录下的所有资源文件夹和资源文件
 */
void fileWatcher::monitorFile()
{
    qCDebug(app) << "monitorFile";
    watcherObj->removePaths(watcherObj->directories());
    watcherObj->removePaths(watcherObj->files());

    QStringList listMonitorFile;
    QStringList listModule;
    QStringList  assetsPathList = Utils::getSystemManualDir();
    qCDebug(app) << "Found" << assetsPathList.size() << "manual directories to monitor";

    foreach (auto assetsPath, assetsPathList) {
        qCDebug(app) << "Processing manual directory:" << assetsPath;
        listModule.append(assetsPath);

        //新文案结构 /usr/share/deepin-manual/manual-assets/[application | system]/appName/appNameT/land/*_appNameT.md
        for (const QString &type : QDir(assetsPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
            //监控资源文件夹
            if (type == "system" || type == "application") {
                QString typePath = assetsPath + "/" + type;
                listModule.append(typePath);
                qCDebug(app) << "Processing manual type:" << typePath;
                //监控资源文件夹
                for (QString &module : QDir(typePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                    //./manual-assets/application(system)/appName
                    QString modulePath = typePath + "/" + module;
                    listModule.append(modulePath);
                    QStringList listAppNameT = QDir(modulePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs);

                    if (listAppNameT.count() != 1) {
                         qCCritical(app) << modulePath  << "：there are more folders..:" << listAppNameT.count();
                        continue;
                    }
                    //./manual-assets/application(system)/appName/appNameT
                    QString appPath = modulePath + "/" + listAppNameT.at(0);
                    listModule.append(appPath);
                    qCDebug(app) << "Processing app:" << appPath;
                    for (QString &lang : QDir(appPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                        if (lang == "zh_CN" || lang == "en_US") {
                            //./manual-assets/application(system)/appName/appNameT/lang
                            QString langPath = appPath + "/" + lang;
                            listModule.append(langPath);
                            qCDebug(app) << "Processing language:" << langPath;
                            for (QString &mdFile : QDir(langPath).entryList(QDir::Files)) {
                                if (mdFile.endsWith("md")) {
                                    //./manual-assets/application(system)/appName/appNameT/lang/md
                                    QString strMd = langPath + "/" + mdFile;
                                    QFileInfo fileinfo(strMd);
                                    if (fileinfo.isSymLink()) {
                                        QString target = fileinfo.symLinkTarget();
                                        qCDebug(app) << "Adding symlink:" << strMd << "->" << target;
                                        listMonitorFile.append(target);
                                    } else {
                                        qCDebug(app) << "Adding markdown file:" << strMd;
                                        listMonitorFile.append(strMd);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        //旧文案结构兼容  /usr/share/deepin-manual/manual-assets/[professional | server]/appName/lang/index.md
#if 1
        for (const QString &system : QDir(assetsPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
            if (systemType.contains(system)) {
                QString typePath = assetsPath + "/" + system;
                listModule.append(typePath);
                qCDebug(app) << "Processing legacy system:" << typePath;

                for (QString &module : QDir(typePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                    QString modulePath = typePath + "/" + module;
                    listModule.append(typePath);
                    for (QString &lang : QDir(modulePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                        if (lang == "zh_CN" || lang == "en_US") {
                            QString strMd = modulePath + "/" + lang + "/index.md";
                            qCDebug(app) << "Adding legacy markdown file:" << strMd;
                            QFileInfo fileinfo(strMd);
                            if (fileinfo.isSymLink()) {
                                QString target = fileinfo.symLinkTarget();
                                qCDebug(app) << "Adding legacy symlink:" << strMd << "->" << target;
                                listMonitorFile.append(target);
                            } else {
                                qCDebug(app) << "Adding legacy markdown file:" << strMd;
                                listMonitorFile.append(strMd);
                            }
                        }
                    }
                }
            }
        }
#endif

        //监控模块资源文件夹
        if (!listModule.isEmpty()) {
            watcherObj->addPaths(listModule);
            qCDebug(app) << "Adding" << listModule.size() << "directories to watch";
        }
        
        //监控资源文件
        if (!listMonitorFile.isEmpty()) {
            watcherObj->addPaths(listMonitorFile);
            qCDebug(app) << "Adding" << listMonitorFile.size() << "files to watch";
        }
    }
    qCDebug(app) << "WatchAllFiles... ...";
}

/**
 * @brief fileMonitor::onChangeFile 文件改变触发槽，
 * @param path 触发文件路径
 * @note QFileSystemWatcher监控文件时，触发改变信号后，会自动结束监控。需重新再次监控此文件。
 */
void fileWatcher::onChangeFile(const QString &path)
{
    qCDebug(app) << path;
    QTimer::singleShot(50, [ = ]() {
        watcherObj->addPath(path);
        qCDebug(app) << "Adding" << path << "to watch";
    });

    qCDebug(app) << "Timer triggered, checking file changes";
    timerObj->start();
}

/**
 * @brief fileMonitor::onChangeDirSlot 文件夹改变触发槽
 * @param path 触发文件夹路径
 */
void fileWatcher::onChangeDirSlot(const QString &path)
{
    qCDebug(app) << path;
    timerObj->start();
}

void fileWatcher::onTimerOut()
{
    qCDebug(app) << "Timer triggered, checking file changes";
    QMap<QString, QString> mapNow;
    QStringList  assetsPathList = Utils::getSystemManualDir();
    foreach (auto assetsPath, assetsPathList) {

        for (const QString &type : QDir(assetsPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
            if (type == "system" || type == "application") {
                QString typePath = assetsPath + "/" + type;
                //监控资源文件夹
                for (QString &module : QDir(typePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                    //./manual-assets/application(system)/appName
                    QString modulePath = typePath + "/" + module;
                    QStringList listAppNameT = QDir(modulePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs);

                    if (listAppNameT.count() != 1) {
                        qCCritical(app) << modulePath  << "：there are more folders..:" << listAppNameT.count();
                        continue;
                    }
                    //./manual-assets/application(system)/appName/appNameT
                    QString appPath = modulePath + "/" + listAppNameT.at(0);
                    for (QString &lang : QDir(appPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                        if (lang == "zh_CN" || lang == "en_US") {
                            //./manual-assets/application(system)/appName/appNameT/lang
                            QString langPath = appPath + "/" + lang;
                            qCDebug(app) << "Checking file changes:" << langPath;
                            for (QString &mdFile : QDir(langPath).entryList(QDir::Files)) {
                                if (mdFile.endsWith("md")) {
                                    //./manual-assets/application(system)/appName/appNameT/lang/md
                                    QString strMd = langPath + "/" + mdFile;
                                    qCDebug(app) << "Checking file changes:" << strMd;
                                    QFileInfo fileInfo(strMd);
                                    if (fileInfo.exists()) {
                                        QString modifyTime = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss.zzz");
                                        mapNow.insert(strMd, modifyTime);
                                        qCDebug(app) << "File changed:" << strMd << "modifyTime:" << modifyTime;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

//旧文案结构兼容  /usr/share/deepin-manual/manual-assets/[professional | server]/appName/lang/index.md
#if 1
        for (const QString &system : QDir(assetsPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
            if (systemType.contains(system)) {
                QString typePath = assetsPath + "/" + system;
                qCDebug(app) << "Checking file changes:" << typePath;
                for (QString &module : QDir(typePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                    QString modulePath = typePath + "/" + module;
                    for (QString &lang : QDir(modulePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                        if (lang == "zh_CN" || lang == "en_US") {
                            QString strMd = modulePath + "/" + lang + "/index.md";
                            qCDebug(app) << "Checking file changes:" << strMd;
                            QFileInfo fileInfo(strMd);
                            if (fileInfo.exists()) {
                                QString modifyTime = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss.zzz");
                                mapNow.insert(strMd, modifyTime);
                                qCDebug(app) << "File changed:" << strMd << "modifyTime:" << modifyTime;
                            }
                        }
                    }
                }
            }
        }
#endif
    }
    QFileInfo fileInfo(kVideoConfigPath);
    if (fileInfo.exists()) {
        QString modifyTime = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss.zzz");
        mapNow.insert(kVideoConfigPath, modifyTime);
        qCDebug(app) << "File changed:" << kVideoConfigPath << "modifyTime:" << modifyTime;
    }

    QStringList deleteList;
    QStringList addList;
    QStringList addTime;
    qCDebug(app) << "Checking file changes";
    checkMap(mapOld, mapNow, deleteList, addList, addTime);
    mapOld = mapNow;
    emit filelistChange(deleteList, addList, addTime);
    monitorFile();
    qCDebug(app) << "File change processing completed";
}
