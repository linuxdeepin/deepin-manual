// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "controller/helpermanager.h"
#include "dbus/dbus_consts.h"
#include "dbus/manual_search_adapter.h"
#include "dbus/manual_search_proxy.h"
#include "dbus/dmanwatcher.h"

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

#ifdef LINGLONG_BUILD
    //玲珑环境添加WebEngine
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

    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--single-process");
#else
    if (!Utils::judgeWayLand()) {
        qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--single-process");
    }
#endif

    // 增加路径以搜索主机应用和玲珑应用中的帮助手册
    if (qEnvironmentVariableIsSet("LINGLONG_APPID")) {
        QByteArray paths = qgetenv("XDG_DATA_DIRS");
        paths.append(":/run/host/rootfs/usr/share:/run/host/rootfs/var/lib/linglong/entries/share");
        qputenv("XDG_DATA_DIRS", paths);
    }

    // 后端服务dmanHelper自检，若前端dman应用不存在，则后端dmanHelper退出
    DManWatcher watcher;
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
