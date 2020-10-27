#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include "base/utils.h"

#include <QObject>

#include <QTimer>
#include <QFileSystemWatcher>


class fileWatcher : public QObject
{
    Q_OBJECT
public:
    //构造函数
    fileWatcher(QObject *parent = nullptr);
    void setFileMap(QMap<QString, QString> &map);
    void checkMap(QMap<QString, QString> &mapOld, QMap<QString, QString> &mapNow, QStringList &deleteList, QStringList &addList, QStringList &addTime);


signals:
    //文件列表发生改变信号
    void filelistChange(QStringList deleteList, QStringList addList, QStringList addTime);

private:
    //文件监控
    void monitorFile();

private slots:
    void onChangeFile(const QString &);
    void onChangeDirSlot(const QString &);
    void onTimerOut();


private:
    QFileSystemWatcher *watcherObj;

    //资源文件路径列表
    QStringList listMonitorFile;

    //资源文件夹路径列表
    QStringList listModule;

    //key:md文件路径 value: md文件更新时间
    QMap<QString, QString> mapOld;

    QTimer *timerObj;
//    QMap<QString,QStringList> mapModule;
};

#endif // FILEWATCHER_H
