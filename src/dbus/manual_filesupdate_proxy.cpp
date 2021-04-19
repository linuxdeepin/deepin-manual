/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     wangmingliang <wangmingliang@uniontech.com>
* Maintainer: wangmingliang <wangmingliang@uniontech.com>
*
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
