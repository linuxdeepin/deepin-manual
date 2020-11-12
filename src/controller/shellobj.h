/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     wujian <wujian@uniontech.com>
* Maintainer: wujian <wujian@uniontech.com>
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef SHELLOBJ_H
#define SHELLOBJ_H

#include <QObject>
#include <QThread>

class shellObj : public QObject
{
    Q_OBJECT
public:
    shellObj();
    /**
     * @brief ~DMailShell 析构函数
     */
    virtual ~shellObj();


    /**
     * @brief execSystem　创建一个DMailShell对象调用ｓｙｓｔｅｍ执行输入的命令
     * @param cmds 命令内容
     * @return  新建的DMailShell对象
     */
    static shellObj &execSystem(const QString &cmds);


private:
    int startSystemThread(const QString &cmd);
    /**
     * @brief runSystem 采用ｓｙｓｔｅｍ来执行ｓｈｅｌｌ命令
     */
    void runSystem();

private:
    /**
     * @brief cmd 保存shell命令
     */
    QString cmd;

    /**
     * @brief thread　线程对象
     */
    QThread *thread = nullptr;

};

#endif // SHELLOBJ_H
