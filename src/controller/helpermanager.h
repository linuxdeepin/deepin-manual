#ifndef HELPERMANAGER_H
#define HELPERMANAGER_H

#include <QObject>
#include "controller/filewatcher.h"
#include "controller/search_db.h"
#include "dpinyin.h"

class helperManager  : public QObject
{
    Q_OBJECT
public:
    helperManager(QObject *parent = nullptr);

private:
    //初始化数据库
    void initDbConfig();
    void getModuleInfo();
    //初始化信号与槽
    void initConnect();
    void handleDb(const QStringList &deleteList, const QStringList &addList, const QStringList &addTime);
    void dbusSend(const QStringList &deleteList, const QStringList &addList);

private slots:
    //文件列表发生改变信号槽
    void onFilelistChange(QStringList deleteList, QStringList addList, QStringList addTime);


private:
    fileWatcher *watcherObj;
    SearchDb *dbObj;
};

#endif // HELPERMANAGER_H
