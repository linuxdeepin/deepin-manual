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

#include <DApplication>
#include <DPlatformWindowHandle>
#include <LogManager.h>
#include <QDBusConnection>
#include <QIcon>
#include <qcef_context.h>
#include <DApplicationSettings>

#include "base/consts.h"
#include "controller/argument_parser.h"
#include "controller/window_manager.h"
#include "resources/themes/images.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char **argv)
{
    qputenv("DXCB_FAKE_PLATFORM_NAME_XCB", "true");
    qputenv("DXCB_REDIRECT_CONTENT", "true");

    QCefGlobalSettings settings;
    // Do not use sandbox.
    settings.setNoSandbox(true);
#ifndef NDEBUG
    // Open http://localhost:9222 in chromium browser to see dev tools.
    settings.setRemoteDebug(true);
    settings.setLogSeverity(QCefGlobalSettings::LogSeverity::Error);
#else
    settings.setRemoteDebug(false);
    settings.setLogSeverity(QCefGlobalSettings::LogSeverity::Error);
#endif
    // Disable GPU process.
    settings.addCommandLineSwitch("--disable-gpu", "");
    // Set web cache folder.
    QDir cache_dir(dman::GetCacheDir());
    cache_dir.mkpath(".");
    settings.setCachePath(cache_dir.filePath("cache"));
    settings.setUserDataPath(cache_dir.filePath("cef-storage"));

    const int exit_code = QCefInit(argc, argv, settings);
    if (exit_code >= 0) {
        return exit_code;
    }

    Dtk::Widget::DApplication::loadDXcbPlugin();

    Dtk::Widget::DApplication app(argc, argv);
    if (!DPlatformWindowHandle::pluginVersion().isEmpty()) {
        app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);
    }

//  app.setTheme("light");
    app.setAttribute(Qt::AA_EnableHighDpiScaling, true);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    app.setWindowIcon(QIcon(dman::kImageDeepinManual));
    app.setProductIcon(QIcon(dman::kImageDeepinManual));
    app.setOrganizationName("deepin");
    app.setOrganizationDomain("deepin.org");
    app.setApplicationVersion(DApplication::buildVersion("2019-10-17"));
    app.setApplicationName(dman::kAppName);
    app.loadTranslator();
    app.setApplicationDisplayName(QObject::tr("Deepin Manual"));
    app.setApplicationDescription(QObject::tr(
                                      "Deepin Manual is designed to help users learn deepin and "
                                      "Deepin applications, providing specific instructions and "
                                      "function descriptions."));
    app.setApplicationAcknowledgementPage(
        "https://www.deepin.org/acknowledgments/deepin-manual/");
    //save theme
    DApplicationSettings dApplicationSettings;

    Dtk::Core::DLogManager::registerFileAppender();
    Dtk::Core::DLogManager::registerConsoleAppender();

    dman::ArgumentParser argument_parser;

    if (argument_parser.parseArguments()) {
        // Exit process after 1000ms.
        QTimer::singleShot(1000, [&]() {
            app.quit();
        });
        return app.exec();
    } else {
        dman::WindowManager window_manager;
        QObject::connect(&argument_parser,
                         &dman::ArgumentParser::openManualRequested,
                         &window_manager, &dman::WindowManager::openManual);
        // Send openManualRequested() signals after slots connected.
        argument_parser.openManualsDelay();

        QCefBindApp(&app);
        return app.exec();
    }
}
