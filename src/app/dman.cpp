// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "base/consts.h"
#include "controller/argument_parser.h"
#include "controller/window_manager.h"
#include "environments.h"
#include "resources/themes/images.h"
#include "controller/shellobj.h"
#include "base/utils.h"
#include "base/eventlogutils.h"
#include "base/logger.h"

#include <DApplication>
#include <DPlatformWindowHandle>
#include <DLog>

#include <QDBusConnection>
#include <QIcon>
#include <QSurfaceFormat>

DWIDGET_USE_NAMESPACE

int main(int argc, char **argv)
{
    // 日志处理要放在app之前，否则QApplication 内部可能进行了日志打印，导致环境变量设置不生效
    MLogger();

    QDateTime time;

    if (!QString(qgetenv("XDG_CURRENT_DESKTOP")).toLower().startsWith("deepin")) {
        setenv("XDG_CURRENT_DESKTOP", "Deepin", 1);
    }
    qputenv("DXCB_FAKE_PLATFORM_NAME_XCB", "true");
    //禁用GPU
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu");

    qputenv("DTK_USE_SEMAPHORE_SINGLEINSTANCE", "1");
#ifdef __mips__
    bool enableChromeFlag = false;
#else
    bool enableChromeFlag = true;
#endif

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

    if (enableChromeFlag) {
        qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--single-process");
    }
#else
    if (enableChromeFlag && !Utils::judgeWayLand()) {
        qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--single-process");
    }
#endif

    //所有进程类型禁用沙箱..此配置开启禁用gpu后无效
//    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--no-sandbox");
    //龙芯机器配置,使得DApplication能正确加载QTWEBENGINE
    qputenv("DTK_FORCE_RASTER_WIDGETS", "FALSE");
    //访问http://127.0.0.1:7777/调试
    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "7777");
    //    Dtk::Widget::DApplication::loadDXcbPlugin();

#ifdef __sw_64__
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--no-sandbox");
#endif

    if (Utils::judgeWayLand()) {
        qputenv("_d_disableDBusFileDialog", "true");
        setenv("PULSE_PROP_media.role", "video", 1);
        QSurfaceFormat format;
        format.setRenderableType(QSurfaceFormat::OpenGLES);
        format.setDefaultFormat(format);
    }

    // 增加路径以搜索主机应用和玲珑应用中的帮助手册
    if (qEnvironmentVariableIsSet("LINGLONG_APPID")) {
        QByteArray paths = qgetenv("XDG_DATA_DIRS");
        paths.append(":/run/host/rootfs/usr/share:/run/host/rootfs/var/lib/linglong/entries/share");
        qputenv("XDG_DATA_DIRS", paths);
    }

    Dtk::Widget::DApplication app(argc, argv);
    if (!DPlatformWindowHandle::pluginVersion().isEmpty()) {
        app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);
    }

    //判断龙芯 20210419
    if (Utils::judgeLoongson()) {
        //add by wujian 20200907 for 解决龙芯平台，QWebEngine因字体库字体太多，造成启动失败的问题
        QString strHomePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        QString strExeShell = QString("rm -fr %1/.cache/fontconfig").arg(strHomePath);
        shellObj::execSystem(strExeShell);
    }

    //设置窗口属性
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    app.setWindowIcon(QIcon::fromTheme("deepin-manual"));
    app.setProductIcon(QIcon::fromTheme("deepin-manual"));
    app.setOrganizationName("deepin");
    app.setOrganizationDomain("deepin.org");
    app.setApplicationVersion(VERSION);
    app.setApplicationName(kAppName);
    app.setDesktopFileName("deepin-manual.desktop");
    app.loadTranslator();
    app.setApplicationDisplayName(QObject::tr("Manual"));
    app.setApplicationDescription(QObject::tr(
                                      "Manual is designed to help users learn the operating system and its applications,"
                                      " providing specific instructions and function descriptions."));
    app.setApplicationAcknowledgementPage("https://www.deepin.org/acknowledgments/deepin-manual/");

    ArgumentParser argument_parser(&app);
    WindowManager window_manager;
    // window_manager.setStartTime(startTime);
    //绑定参数解析 信号与槽
    QObject::connect(&argument_parser, &ArgumentParser::openManualWithSearchRequested,
                     &window_manager, &WindowManager::openManualWithSearch);
    QObject::connect(&argument_parser, &ArgumentParser::openManualRequested,
                     &window_manager, &WindowManager::openManual);

    //解析启动参数
    if (!argument_parser.parseArguments()) {
        qDebug() << "argument_parser.parseArguments()";
        //解析参数失败，１００ｍｓ退出进程
        QTimer::singleShot(100, [&]() {
            app.quit();
        });
        return app.exec();
    }
    argument_parser.openManualsDelay();

    // init logger format
    MLogger::initLogger();

    //埋点记录启动数据
    QJsonObject objStartEvent{
        {"tid", Eventlogutils::StartUp},
        {"version", VERSION},
        {"mode", 1},
    };
    Eventlogutils::GetInstance()->writeLogs(objStartEvent);
    // fix error for cutelogger
    // No appenders associated with category js
    // 日志相关
    auto category = "js";
    auto fileAppender =
        new Dtk::Core::RollingFileAppender(Dtk::Core::DLogManager::getlogFilePath());
    qInfo() << Dtk::Core::DLogManager::getlogFilePath() << fileAppender->datePatternString();
    static Dtk::Core::Logger customLoggerInstance(category);
    customLoggerInstance.logToGlobalInstance(category, true);
    customLoggerInstance.registerAppender(fileAppender);

    return app.exec();
}
