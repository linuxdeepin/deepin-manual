#include "filewatcher.h"
#include "base/utils.h"

#include <QDir>
#include <QDebug>
#include <DSysInfo>

fileWatcher::fileWatcher(QObject *parent)
    : QObject(parent)
    , watcherObj(new QFileSystemWatcher)
    , timerObj(new QTimer)
{
    timerObj->setSingleShot(true);
    timerObj->setInterval(3 * 1000);
    connect(timerObj, &QTimer::timeout, this, &fileWatcher::onTimerOut);
    connect(watcherObj, &QFileSystemWatcher::fileChanged, this, &fileWatcher::onChangeFile);
    connect(watcherObj, &QFileSystemWatcher::directoryChanged, this, &fileWatcher::onChangeDirSlot);
    monitorFile();
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

    for (const QString &mdPath : listOldMd) {
        if (!listNowMd.contains(mdPath)) {
            deleteList.append(mdPath);
        }
    }

    for (const QString &mdPath : listNowMd) {
        if (!listOldMd.contains(mdPath)) {
            addList.append(mdPath);
        } else if (mapOld.value(mdPath) != mapNow.value(mdPath)) {
            addList.append(mdPath);
        }
    }

    if (!addList.isEmpty()) {
        for (const QString &file : addList) {
            addTime.append(mapNow.value(file));
        }
    }
}


/**
 * @brief fileWatcher::monitorFile 监控特定目录下的所有资源文件夹和资源文件
 */
void fileWatcher::monitorFile()
{
    watcherObj->removePaths(watcherObj->directories());
    watcherObj->removePaths(watcherObj->files());

    QStringList listMonitorFile;
    QStringList listModule;
    QString  assetsPath = Utils::getSystemManualDir();
    listModule.append(assetsPath);
    for (const QString &type : QDir(assetsPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
        //监控资源文件夹
        if (type == "system" || type == "application") {
            QString typePath = assetsPath + "/" + type;
            listModule.append(typePath);
            //监控资源文件夹
            for (QString &module : QDir(typePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                //./manual-assets/application(system)/appName
                QString modulePath = typePath + "/" + module;
                listModule.append(modulePath);
                QStringList listAppNameT = QDir(modulePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs);
                if (listAppNameT.count() != 1) {
                    qCritical() << Q_FUNC_INFO << modulePath << listAppNameT << "：there are more folders..";
                }
                //./manual-assets/application(system)/appName/appNameT
                QString appPath = modulePath + "/" + listAppNameT.at(0);
                listModule.append(appPath);
                for (QString &lang : QDir(appPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                    if (lang == "zh_CN" || lang == "en_US") {
                        //./manual-assets/application(system)/appName/appNameT/lang
                        QString langPath = appPath + "/" + lang;
                        listModule.append(langPath);
                        for (QString &mdFile : QDir(langPath).entryList(QDir::Files)) {
                            if (mdFile.endsWith("md")) {
                                //./manual-assets/application(system)/appName/appNameT/lang/md
                                QString strMd = langPath + "/" + mdFile;
                                listMonitorFile.append(strMd);
                            }
                        }
                    }
                }
            }
        }
    }

    //监控模块资源文件夹
    if (!listModule.isEmpty()) {
        watcherObj->addPaths(listModule);
    }
    //监控资源文件
    if (!listMonitorFile.isEmpty()) {
        watcherObj->addPaths(listMonitorFile);
    }
    qDebug() << Q_FUNC_INFO << "WatchAllFiles... ...";
}

/**
 * @brief fileMonitor::onChangeFile 文件改变触发槽，
 * @param path 触发文件路径
 * @note QFileSystemWatcher监控文件时，触发改变信号后，会自动结束监控。需重新再次监控此文件。
 */
void fileWatcher::onChangeFile(const QString &path)
{
    qDebug() << Q_FUNC_INFO << path;
    QTimer::singleShot(50, [ = ]() {
        watcherObj->addPath(path);
    });

    timerObj->start();
}

/**
 * @brief fileMonitor::onChangeDirSlot 文件夹改变触发槽
 * @param path 触发文件夹路径
 */
void fileWatcher::onChangeDirSlot(const QString &path)
{
    qDebug() << Q_FUNC_INFO << path;
    timerObj->start();
}

void fileWatcher::onTimerOut()
{
    QMap<QString, QString> mapNow;
    QString  assetsPath = Utils::getSystemManualDir();

    for (const QString &type : QDir(assetsPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
        if (type == "system" || type == "application") {
            QString typePath = assetsPath + "/" + type;
            //监控资源文件夹
            for (QString &module : QDir(typePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                //./manual-assets/application(system)/appName
                QString modulePath = typePath + "/" + module;
                QStringList listAppNameT = QDir(modulePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs);
                if (listAppNameT.count() != 1) {
                    qCritical() << modulePath << listAppNameT << "：there are more folders..";
                }
                //./manual-assets/application(system)/appName/appNameT
                QString appPath = modulePath + "/" + listAppNameT.at(0);
                for (QString &lang : QDir(appPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                    if (lang == "zh_CN" || lang == "en_US") {
                        //./manual-assets/application(system)/appName/appNameT/lang
                        QString langPath = appPath + "/" + lang;
                        for (QString &mdFile : QDir(langPath).entryList(QDir::Files)) {
                            if (mdFile.endsWith("md")) {
                                //./manual-assets/application(system)/appName/appNameT/lang/md
                                QString strMd = langPath + "/" + mdFile;
                                QFileInfo fileInfo(strMd);
                                if (fileInfo.exists()) {
                                    QString modifyTime = fileInfo.lastModified().toString();
                                    mapNow.insert(strMd, modifyTime);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    QStringList deleteList;
    QStringList addList;
    QStringList addTime;
    checkMap(mapOld, mapNow, deleteList, addList, addTime);
    mapOld = mapNow;
    emit filelistChange(deleteList, addList, addTime);
    monitorFile();
}
