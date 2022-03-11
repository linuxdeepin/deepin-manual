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
#include "controller/helpermanager.h"
#include "dbus/dbus_consts.h"
#include "dbus/manual_search_adapter.h"
#include "dbus/manual_search_proxy.h"

#include <DLog>

#include <QWebEngineView>
#include <QApplication>
#include <QDebug>
#include <QOpenGLContext>
#include <QSurfaceFormat>

int main(int argc, char **argv)
{
    //欧拉版root用户登录时会报no-sandbox错误的问题,增加此参数后使qtwebengine进程与主进程合并
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--single-process");
    QApplication app(argc, argv);

//    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "7777");

    ManualSearchProxy search_obj;
    ManualSearchAdapter adapter(&search_obj);

    qDebug() << Dtk::Core::DLogManager::getlogFilePath();

#ifndef __sw_64__
    QOpenGLContext ctx;
    QSurfaceFormat fmt;
    fmt.setRenderableType(QSurfaceFormat::OpenGL);
    ctx.setFormat(fmt);
    ctx.create();
    if (!ctx.isValid()) {
       fmt.setRenderableType(QSurfaceFormat::OpenGLES);
    }
    fmt.setDefaultFormat(fmt);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu");
#endif

    Dtk::Core::DLogManager::registerFileAppender();
    Dtk::Core::DLogManager::registerConsoleAppender();

    QDBusConnection conn = QDBusConnection::sessionBus();
    if (!conn.registerService(kManualSearchService)
            || !conn.registerObject(kManualSearchIface, &search_obj)) {
        qCritical() << "dmanHelper failed to register dbus service";
        return -1;
    } else {
        qDebug() << "dmanHelper register dbus service success!";
    }

    helperManager obj;

    return app.exec();
}
