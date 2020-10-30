/*
   * Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
   *
   * Author:     xxx <xxx@uniontech.com>
   *
   * Maintainer: xxx <xxx@uniontech.com>
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
#ifndef HELPERMANAGER_H
#define HELPERMANAGER_H

#include <QObject>
#include "dpinyin.h"

class fileWatcher;
class SearchDb;
class helperManager  : public QObject
{
    Q_OBJECT
public:
    helperManager(QObject *parent = nullptr);

private:
    //初始化数据库
    void initDbConfig();
    //白名单对比
    void getModuleInfo();
    //初始化信号与槽
    void initConnect();
    //对修改的文件列表更新数据库
    void handleDb(const QStringList &deleteList, const QStringList &addList, const QStringList &addTime);
    //同时前端刷新
    void dbusSend(const QStringList &deleteList, const QStringList &addList);

private slots:
    //文件列表发生改变信号槽
    void onFilelistChange(QStringList deleteList, QStringList addList, QStringList addTime);


private:
    //文件监控对象
    fileWatcher *watcherObj;
    //数据库对象
    SearchDb *dbObj;
};

#endif // HELPERMANAGER_H
