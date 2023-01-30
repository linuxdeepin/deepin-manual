// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "manual_filesupdate_proxy.h"
#include <QDebug>

ManualFilesUpdateProxy::ManualFilesUpdateProxy(QObject *parent) : QObject(parent)
{
    this->setObjectName("ManualFilesUpdateProxy");
}

ManualFilesUpdateProxy::~ManualFilesUpdateProxy()
{

}

/**
 * @brief ManualFilesUpdateProxy::OnFilesUpdate
 * @node 文件更新
 * @param list
 */
void ManualFilesUpdateProxy::OnFilesUpdate(const QStringList &list)
{
    qDebug() << Q_FUNC_INFO << list;

    //发送信号->WindowManager::onFilesUpdate
    emit this->FilesUpdate(list);
    return;
}
