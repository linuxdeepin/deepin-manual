// SPDX-FileCopyrightText: 2022-2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "controller/helpermanager.h"
#include "dbus/dbus_consts.h"
#include "dbus/manual_search_adapter.h"
#include "dbus/manual_search_proxy.h"
#include "dbus/dmanwatcher.h"

#include <DLog>
#include "base/utils.h"
#include "base/logger.h"
#include <QLoggingCategory>
#include <QWebEngineView>
#include <QApplication>
#include <QDebug>
#include <QOpenGLContext>
#include <QSurfaceFormat>

// main() 内有局部对象 app（QApplication），与 ddlog.h 的分类变量 app 同名冲突，
// 此处单独声明同分类名（org.deepin.manual）的分类变量
Q_LOGGING_CATEGORY(dmanApp, "org.deepin.manual")

int main(int argc, char **argv)
{
    //欧拉版root用户登录时会报no-sandbox错误的问题,增加此参数后使qtwebengine进程与主进程合并

    // 日志处理要放在app之前，否则QApplication内部可能进行了日志打印，导致环境变量设置不生效
    // 注意：MLogger 实例必须常驻（static），若用临时对象，析构时会断开 DConfig valueChanged
    // 连接，导致调试模式运行中动态切换日志级别不生效
    static MLogger logger;

    QApplication app(argc, argv);

    qputenv("DXCB_FAKE_PLATFORM_NAME_XCB", "true");
    qCDebug(dmanApp) << "Set DXCB_FAKE_PLATFORM_NAME_XCB=true";

#ifdef __sw_64__
    // sw_64 requires combined Chromium flags to avoid being overwritten
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu --single-process --js-flags=--jitless");
    qCDebug(dmanApp) << "Set QTWEBENGINE_CHROMIUM_FLAGS for sw_64: --disable-gpu --single-process --js-flags=--jitless";
#else
    //禁用GPU
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu");
    qCDebug(dmanApp) << "Set QTWEBENGINE_CHROMIUM_FLAGS=--disable-gpu";
#endif
//    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "7777");

#ifdef LINGLONG_BUILD
    //玲珑环境添加WebEngine
    QString webEngineProcessPath = DMAN_QWEBENGINE_DIR"" + QLibraryInfo::location(QLibraryInfo::LibrariesPath).mid(
                QLibraryInfo::location(QLibraryInfo::LibrariesPath).lastIndexOf("/")) + QString("/qt5/libexec/QtWebEngineProcess");
    QFile file(webEngineProcessPath);
    if (file.exists())
        qputenv("QTWEBENGINEPROCESS_PATH", webEngineProcessPath.toLocal8Bit());
    else
        qCWarning(dmanApp) << "qputenv QTWEBENGINEPROCESS_PATH fail";

    QFile fileTs(DMAN_WEBENGINERES_DIR);
    QFile fileRes(DMAN_WEBENGINETS_DIR);
    if (fileTs.exists() && fileRes.exists())
        qputenv("QTWEBENGINERESOURCE_PATH", (DMAN_WEBENGINETS_DIR":" + QString(DMAN_WEBENGINERES_DIR)).toStdString().c_str());
    else
        qCWarning(dmanApp) << "qputenv QTWEBENGINERESOURCE_PATH fail";

    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--single-process");
#else
#ifndef __sw_64__
    // sw_64 already has complete flags set above, skip override here
    if (!Utils::judgeWayLand()) {
        qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--single-process");
        qCDebug(dmanApp) << "Non-Wayland environment, set single-process mode";
    } else {
        qCDebug(dmanApp) << "Wayland environment detected";
    }
#endif
#endif

    // 增加路径以搜索主机应用和玲珑应用中的帮助手册
    if (qEnvironmentVariableIsSet("LINGLONG_APPID")) {
        qCDebug(dmanApp) << "Running in Linglong container environment";
        QByteArray paths = qgetenv("XDG_DATA_DIRS");
        qCDebug(dmanApp) << "Original XDG_DATA_DIRS:" << paths;
        paths.append(":/run/host/rootfs/usr/share:/run/host/rootfs/var/lib/linglong/entries/share");
        qputenv("XDG_DATA_DIRS", paths);
    }
    qCDebug(dmanApp) << "XDG_DATA_DIRS set to:" << qgetenv("XDG_DATA_DIRS");

    // 后端服务dmanHelper自检，若前端dman应用不存在，则后端dmanHelper退出
    DManWatcher watcher;
    ManualSearchProxy search_obj;
    ManualSearchAdapter adapter(&search_obj);

    QString logPath = Dtk::Core::DLogManager::getlogFilePath();
    qCDebug(dmanApp) << "Log file path:" << logPath;

    qCDebug(dmanApp) << "Initializing OpenGL context";
    QOpenGLContext ctx;
    QSurfaceFormat fmt;
    fmt.setRenderableType(QSurfaceFormat::OpenGL);
    qCDebug(dmanApp) << "Setting OpenGL renderable type";
    ctx.setFormat(fmt);
    qCDebug(dmanApp) << "Creating OpenGL context";
    ctx.create();
    if (!ctx.isValid()) {
        qCWarning(dmanApp) << "OpenGL context invalid, fallback to OpenGLES";
        fmt.setRenderableType(QSurfaceFormat::OpenGLES);
        qCDebug(dmanApp) << "OpenGLES format set, profile:" << fmt.profile();
    } else {
        qCDebug(dmanApp) << "OpenGL context created successfully";
    }
    fmt.setDefaultFormat(fmt);
    fmt.setProfile(QSurfaceFormat::CoreProfile);

    Dtk::Core::DLogManager::registerFileAppender();
    Dtk::Core::DLogManager::registerConsoleAppender();

    qCInfo(dmanApp) << "Starting DBus service registration";
    QDBusConnection conn = QDBusConnection::sessionBus();
    if (!conn.registerService(kManualSearchService)
            || !conn.registerObject(kManualSearchIface, &search_obj)) {
        qCCritical(dmanApp) << "dmanHelper failed to register dbus service";
        return -1;
    } else {
        qCDebug(dmanApp) << "dmanHelper register dbus service success!";
    }
    qCInfo(dmanApp) << "Successfully registered DBus service:" << kManualSearchService
           << "and object path:" << kManualSearchIface;

    helperManager obj;

    int ret = app.exec();
    qCInfo(dmanApp) << "dmanHelper exiting with code:" << ret;
    return ret;
}
