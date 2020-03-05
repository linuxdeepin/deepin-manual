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

#include "dbus/manual_search_proxy.h"
#include "dbus/dbus_consts.h"

#include <DLog>
#include <QDir>
#include <QtDBus/QtDBus>

ManualSearchProxy::ManualSearchProxy(QObject *parent)
    : QObject(parent)
    , pManagerObj(nullptr)
{
    this->setObjectName("ManualSearchProxy");
}

ManualSearchProxy::~ManualSearchProxy() {}

void ManualSearchProxy::setManagerObj(windowManager *pObj)
{
    pManagerObj = pObj;
}

bool ManualSearchProxy::ManualExists(const QString &app_name)
{
    QString strManualPath = DMAN_MANUAL_DIR;
    int nType = Dtk::Core::DSysInfo::deepinType();
    if (Dtk::Core::DSysInfo::DeepinServer == (Dtk::Core::DSysInfo::DeepinType)nType) {
        //        strManualPath += "/server";
        strManualPath += "/professional";
    } else {
        //        strManualPath += "/professional";
        strManualPath += "/server";
    }

    QDir manual_dir(strManualPath);
    return manual_dir.exists(app_name);
}

void ManualSearchProxy::BindManual(const QString &app_name, const QString &winId)
{
    qDebug() << Q_FUNC_INFO;
    emit bindManual(app_name, winId);
}

void ManualSearchProxy::CloseManual(const QString &app_name)
{
    emit closeManual(app_name);
}

bool ManualSearchProxy::OnNewWindowOpen(const QString &winId)
{
    bool bRet = false;
    if (pManagerObj) {
        bRet = pManagerObj->newWindowOpen(winId);
    }
    return bRet;
}
