/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
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

#include "dbus/manual_open_proxy.h"
#include "dbus_consts.h"

#include <DLog>
#include <QtDBus/QtDBus>



ManualOpenProxy::ManualOpenProxy(QObject *parent)
    : QObject(parent)
{
    this->setObjectName("ManualOpenProxy");
}

ManualOpenProxy::~ManualOpenProxy() {}

/**
 * @brief ManualOpenProxy::Open
 * @param app_name
 * @note DBus接口，用于打开给定应用名称的帮助文档内容
 */
void ManualOpenProxy::Open(const QString &app_name)
{
    qDebug() << Q_FUNC_INFO << app_name;
    emit this->openManualRequested(app_name, "");
}

void ManualOpenProxy::OpenTitle(const QString &app_name, const QString &title_name)
{
    qDebug() << Q_FUNC_INFO << app_name << "---" << title_name;
    emit this->openManualRequested(app_name, title_name);
}

void ManualOpenProxy::ShowManual(const QString &app_name)
{
    this->Open(app_name);
}

/**
 * @brief ManualOpenProxy::Search
 * @param keyword
 * @note DBus接口，根据给定的关键字发起搜索请求
 */
void ManualOpenProxy::Search(const QString &keyword)
{
    qDebug() << Q_FUNC_INFO << keyword;
    emit this->searchRequested(keyword);
}
