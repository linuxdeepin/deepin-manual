#include "filewatcher.h"

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


//TODO 监控文件
void fileWatcher::monitorFile()
{
    QString  assetsPath = Utils::getSystemManualDir();
    //监控资源文件夹
    watcherObj->addPath(assetsPath);
    for (QString &module : QDir(assetsPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
        QString modulePath = assetsPath + "/" + module;
        listModule.append(modulePath);
        QStringList listLang;
        for (QString &lang : QDir(modulePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
            if (lang == "zh_CN" || lang == "en_US") {
                listLang.append(lang);
                QString strMd = modulePath + "/" + lang + "/index.md";
                listMonitorFile.append(strMd);
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
    //监控资源文件夹
    for (QString &module : QDir(assetsPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
        QString modulePath = assetsPath + "/" + module;
        QStringList listLang;
        for (QString &lang : QDir(modulePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
            if (lang == "zh_CN" || lang == "en_US") {
                listLang.append(lang);
                QString strMd = modulePath + "/" + lang + "/index.md";
                QFileInfo fileInfo(strMd);
                if (fileInfo.exists()) {
                    QString modifyTime = fileInfo.lastModified().toString();
                    mapNow.insert(strMd, modifyTime);
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
    this->monitorFile();
}
