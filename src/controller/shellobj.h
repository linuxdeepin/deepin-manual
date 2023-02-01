// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
