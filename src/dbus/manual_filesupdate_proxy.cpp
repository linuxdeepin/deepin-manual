// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "manual_filesupdate_proxy.h"
#include "base/ddlog.h"

ManualFilesUpdateProxy::ManualFilesUpdateProxy(QObject *parent) : QObject(parent)
{
    this->setObjectName("ManualFilesUpdateProxy");
    qCDebug(app) << "ManualFilesUpdateProxy initialized";
}

ManualFilesUpdateProxy::~ManualFilesUpdateProxy()
{
    qCDebug(app) << "ManualFilesUpdateProxy destroyed";
}

/**
 * @brief ManualFilesUpdateProxy::OnFilesUpdate
 * @node 文件更新
 * @param list
 */
void ManualFilesUpdateProxy::OnFilesUpdate(const QStringList &list)
{
    qCDebug(app) << "Received files update request, file count:" << list.size();
    //发送信号->WindowManager::onFilesUpdate
    emit this->FilesUpdate(list);
    qCDebug(app) << "Files update signal emitted";
    return;
}
