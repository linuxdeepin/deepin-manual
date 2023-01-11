// SPDX-FileCopyrightText: 2019 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef HELPERMANAGER_H
#define HELPERMANAGER_H

#include "dpinyin.h"

#include <QObject>
#include <QWebEngineView>
#include <QWebChannel>
#include <DSysInfo>

class fileWatcher;
class SearchDb;
class JsContext;
class helperManager  : public QObject
{
    Q_OBJECT
public:
    explicit helperManager(QObject *parent = nullptr);

private:

    void initWeb();
    //初始化数据库
    void initDbConfig();
    //白名单对比
    void getModuleInfo();
    //初始化信号与槽
    void initConnect();
    //对修改的文件列表更新数据库
    void handleDb(const QStringList &deleteList, const QStringList &addList, const QStringList &addTime);
    //通知前端刷新
    void dbusSend(const QStringList &deleteList, const QStringList &addList);
    //判断返回md文件符合优先选择。
    QStringList handlePriority(const QStringList &list);

private slots:
    //文件列表发生改变信号槽
    void onFilelistChange(QStringList deleteList, QStringList addList, QStringList addTime);
    void webLoadFinish(bool ok);

    //JS层返回解析内容处理槽
    void onRecvParseMsg(const QString &msg, const QString &path);

    //定时器超时触发槽
    void onTimerOut();


private:
    //文件监控对象
    fileWatcher *watcherObj;
    //数据库对象
    SearchDb *dbObj;
    //定时器对象
    QTimer *timerObj;

    QWebEngineView *m_webView = nullptr;
    QWebChannel *m_webChannel;
    JsContext *jsObj;
    QStringList deleteTList;
    QStringList addTList;
};

#endif // HELPERMANAGER_H
