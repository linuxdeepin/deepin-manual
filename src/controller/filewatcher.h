// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QFileSystemWatcher>

//p表示桌面专业版,h表示个人版，d表示社区版,s表示服务器版，e表示服务器企业版，eu表示服务器欧拉版，i表示服务器行业版, edu表示教育版
//klu表示KelvinU项目版本，pgv表示PanguV项目版本。
const QStringList systemList = {"p", "h", "d", "s", "e", "eu", "i", "klu", "pgv", "edu"};
const QStringList systemType = {"professional", "server", "community", "personal"};

class Utils;
class fileWatcher : public QObject
{
    Q_OBJECT
public:
    //构造函数
    explicit fileWatcher(QObject *parent = nullptr);
    void setFileMap(QMap<QString, QString> &map);

    //文件列表信息对比
    void checkMap(QMap<QString, QString> &mapOld, QMap<QString, QString> &mapNow, QStringList &deleteList, QStringList &addList, QStringList &addTime);

signals:
    //文件列表发生改变信号
    void filelistChange(QStringList deleteList, QStringList addList, QStringList addTime);

private:
    //文件监控
    void monitorFile();

private slots:
    //监控文件发生改变触发槽
    void onChangeFile(const QString &);
    //监控文件夹发生改变触发槽
    void onChangeDirSlot(const QString &);
    //定时器超时触发槽
    void onTimerOut();


private:
    //监控类对象
    QFileSystemWatcher *watcherObj;

    //key:md文件路径 value: md文件更新时间
    QMap<QString, QString> mapOld;

    //定时器对象
    QTimer *timerObj;
};

#endif // FILEWATCHER_H
