#include "filewatcher.h"

#include <QDir>
#include <QDebug>
#include <QTimer>
#include <DSysInfo>


fileWatcher::fileWatcher(QObject *parent)
    : QObject(parent)
    ,watcherObj(new QFileSystemWatcher)
{

}

//TODO 监控文件
void fileWatcher::monitorFile()
{
    QString  assetsPath = Utils::getSystemManualDir();
    //监控资源文件夹
    watcherObj->addPath(assetsPath);
    for (QString &module : QDir(assetsPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
        listModule.append(module);
        QString modulePath = assetsPath + "/" + module;
        for (QString &lang : QDir(modulePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
            if (lang == "zh_CN" || lang == "en_US") {
                QString strMd = modulePath + "/" + lang + "/index.md";
                listMonitorFile.append(strMd);
            }
        }
    }

    //监控资源文件
    if (!listMonitorFile.isEmpty()) {
        watcherObj->addPaths(listMonitorFile);
    }

    connect(watcherObj, &QFileSystemWatcher::fileChanged, this, &fileWatcher::onChangeFile);
    connect(watcherObj, &QFileSystemWatcher::directoryChanged, this, &fileWatcher::onChangeDirSlot);
}

/**
 * @brief fileMonitor::onChangeFile 文件改变触发槽，
 * @param path 触发文件路径
 * @note QFileSystemWatcher监控文件时，触发改变信号后，会自动结束监控。需重新再次监控此文件。
 */
void fileWatcher::onChangeFile(const QString &path)
{
    qDebug() << Q_FUNC_INFO<<path;
    QTimer::singleShot(50, [ = ]() {
        watcherObj->addPath(path);
    });

    //内容拆分
//    QString out, err;
//    const QStringList cmd = {"./web/toSearchIndex.js", "-f", path};
//    const bool ok = dman::SpawnCmd("./web/node", cmd, out, err);
//    qDebug() << "========>" <<ok<< out;
}

/**
 * @brief fileMonitor::onChangeDirSlot 文件夹改变触发槽
 * @param path 触发文件夹路径
 */
void fileWatcher::onChangeDirSlot(const QString &path)
{
    qDebug() << Q_FUNC_INFO<<path;
//    QStringList newList =  QDir(assetsPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs);

//    QStringList resultList;
    //    moduleCheck ckeck = compareModele(newList, resultList);
}
