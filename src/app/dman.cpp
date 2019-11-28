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

#include <QDesktopWidget>
#include <QDBusConnection>
#include <QIcon>
#include <qcef_context.h>
#include <qcef_web_settings.h>
#include <qcef_global_settings.h>

#include "base/consts.h"
#include "view/web_window.h"
#include "controller/argument_parser.h"
#include "controller/window_manager.h"
#include "resources/themes/images.h"
#include "environments.h"

#include <DLog>
#include <DApplication>
#include <DPlatformWindowHandle>
#include <DApplicationSettings>

DWIDGET_USE_NAMESPACE

// namespace
namespace dman
{
    const char kEnableDomStorageFlush[] = "--enable-aggressive-domstorage-flushing";
    const char kDisableGpu[] = "--disable-gpu";
    const char kEnableLogging[] = "--enable-logging";
    const char kLogLevel[] = "--log-level";
}

int main(int argc, char **argv)
{
    qputenv("DXCB_FAKE_PLATFORM_NAME_XCB", "true");

    QCefGlobalSettings settings;
    // Do not use sandbox.
    settings.setNoSandbox(true);

    if (qEnvironmentVariableIntValue("QCEF_DEBUG") == 1) {
        // Open http://localhost:9222 in chromium browser to see dev tools.
        qDebug() << "enable QCEF_DEBUG " << endl;
        settings.setRemoteDebug(true);
        settings.setLogSeverity(QCefGlobalSettings::LogSeverity::Verbose);
    } else {
        qDebug() << "disable QCEF_DEBUG " << endl;
        settings.setRemoteDebug(false);
        settings.setLogSeverity(QCefGlobalSettings::LogSeverity::Error);
    }

    // Disable GPU process.
    settings.addCommandLineSwitch(dman::kDisableGpu, "");

    // Enable aggressive storage commit to minimize data loss.
    // See public/common/content_switches.cc.
    settings.addCommandLineSwitch(dman::kEnableDomStorageFlush, "");

    // Set web cache folder.
    QDir cache_dir(dman::GetCacheDir());
    cache_dir.mkpath(".");
    settings.setCachePath(cache_dir.filePath("cache"));
    settings.setUserDataPath(cache_dir.filePath("cef-storage"));

    // TODO: Rotate console log.
    settings.setLogFile(cache_dir.filePath("web-console.log"));
    settings.addCommandLineSwitch(dman::kEnableLogging, "");
    settings.addCommandLineSwitch(dman::kLogLevel, "0");
    settings.addCommandLineSwitch("--use-views", "");

    const int exit_code = QCefInit(argc, argv, settings);
    if (exit_code >= 0) {
        return exit_code;
    }

    Dtk::Widget::DApplication::loadDXcbPlugin();

    Dtk::Widget::DApplication app(argc, argv);
    if (!DPlatformWindowHandle::pluginVersion().isEmpty()) {
        app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);
    }

//    app.setAttribute(Qt::AA_EnableHighDpiScaling, true);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    app.setWindowIcon(QIcon::fromTheme("deepin-manual"));
    app.setProductIcon(QIcon::fromTheme("deepin-manual"));
    app.setOrganizationName("deepin");
    app.setOrganizationDomain("deepin.org");
    app.setApplicationVersion(VERSION);
    app.setApplicationName(dman::kAppName);
    app.loadTranslator();
    app.setApplicationDisplayName(QObject::tr("Manual"));
    app.setApplicationDescription(QObject::tr("Manual is designed to help users learn UOS and its applications,"
                                              " providing specific instructions and function descriptions."));
    app.setApplicationAcknowledgementPage(
        "https://www.deepin.org/acknowledgments/deepin-manual/");

    //save theme
    DApplicationSettings dApplicationSettings;

    Dtk::Core::DLogManager::registerFileAppender();
    Dtk::Core::DLogManager::registerConsoleAppender();

    // fix error for cutelogger
    // No appenders associated with category js
    auto category = "js";
    auto fileAppender = new Dtk::Core::RollingFileAppender(Dtk::Core::DLogManager::getlogFilePath());
    static Dtk::Core::Logger customLoggerInstance(category);
    customLoggerInstance.logToGlobalInstance(category, true);
    customLoggerInstance.registerAppender(fileAppender);

    dman::ArgumentParser argument_parser;

    if (argument_parser.parseArguments()) {
        qDebug() << "argument_parser.parseArguments()";
        // Exit process after 1000ms.
        QTimer::singleShot(1000, [&]() {
            app.quit();
        });
        return app.exec();
    } else {
        QCefBindApp(&app);

        qDebug() << "argument_parser.openManualsDelay()";
        dman::WindowManager window_manager;
        QObject::connect(&argument_parser,
                         &dman::ArgumentParser::openManualRequested,
                         &window_manager,
                         &dman::WindowManager::openManual);
        // Send openManualRequested() signals after slots connected.
        argument_parser.openManualsDelay();

        return app.exec();
    }
}
