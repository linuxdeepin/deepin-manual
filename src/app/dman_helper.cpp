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
#include "base/utils.h"
#include <QWebEngineView>
#include <QApplication>
#include <QDebug>
#include <QOpenGLContext>
#include <QSurfaceFormat>

int main(int argc, char **argv)
{
    //欧拉版root用户登录时会报no-sandbox错误的问题,增加此参数后使qtwebengine进程与主进程合并

    QApplication app(argc, argv);

    qputenv("DXCB_FAKE_PLATFORM_NAME_XCB", "true");
    //禁用GPU
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu");
//    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "7777");
    //玲珑环境添加WebEngine
    if (Utils::judgeLingLong()) {
        QString webEngineProcessPath = DMAN_QWEBENGINE_DIR"" + QLibraryInfo::location(QLibraryInfo::LibrariesPath).mid(
                                           QLibraryInfo::location(QLibraryInfo::LibrariesPath).lastIndexOf("/")) + QString("/qt5/libexec/QtWebEngineProcess");
        QFile file(webEngineProcessPath);
        if (file.exists())
            qputenv("QTWEBENGINEPROCESS_PATH", webEngineProcessPath.toLocal8Bit());
        else
            qWarning() << "qputenv QTWEBENGINEPROCESS_PATH fail";

        QFile fileTs(DMAN_WEBENGINERES_DIR);
        QFile fileRes(DMAN_WEBENGINETS_DIR);
        if (fileTs.exists() && fileRes.exists())
            qputenv("QTWEBENGINERESOURCE_PATH", (DMAN_WEBENGINETS_DIR":" + QString(DMAN_WEBENGINERES_DIR)).toStdString().c_str());
        else
            qWarning() << "qputenv QTWEBENGINERESOURCE_PATH fail";
    }

    if (!Utils::judgeWayLand()) {
        qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--single-process");
    }
    ManualSearchProxy search_obj;
    ManualSearchAdapter adapter(&search_obj);

    qDebug() << Dtk::Core::DLogManager::getlogFilePath();

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
