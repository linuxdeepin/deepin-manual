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

#include <qcef_context.h>
#include <qcef_global_settings.h>
#include <qcef_web_settings.h>
#include <QDBusConnection>
#include <QDesktopWidget>
#include <QIcon>

#include "base/consts.h"
#include "base/utils.h"
#include "controller/argument_parser.h"
#include "controller/window_manager.h"
#include "environments.h"
#include "resources/themes/images.h"
#include "view/web_window.h"

#include <DApplication>
#include <DApplicationSettings>
#include <DLog>
#include <DPlatformWindowHandle>

DWIDGET_USE_NAMESPACE

int main(int argc, char **argv)
{
    //    qputenv("QCEF_DEBUG", "1");
    qputenv("DXCB_FAKE_PLATFORM_NAME_XCB", "true");
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu");
    qputenv("DTK_FORCE_RASTER_WIDGETS", "FALSE");

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

    app.setApplicationDescription(QObject::tr(
                                      "Manual is designed to help users learn the operating system and its applications,"
                                      " providing specific instructions and function descriptions."));
    app.setApplicationAcknowledgementPage("https://www.deepin.org/acknowledgments/deepin-manual/");

    dman::ArgumentParser argument_parser;
    dman::WindowManager window_manager;
    QObject::connect(&argument_parser, &dman::ArgumentParser::onNewAppOpen, &window_manager,
                     &dman::WindowManager::onNewAppOpen);

    if (argument_parser.parseArguments()) {
        qDebug() << "argument_parser.parseArguments()";
        // Exit process after 1000ms.
        QTimer::singleShot(1000, [&]() {
            app.quit();
        });
        return app.exec();
    }

    qDebug() << "argument_parser.openManualsDelay()";
    QObject::connect(&argument_parser, &dman::ArgumentParser::openManualRequested, &window_manager,
                     &dman::WindowManager::openManual);
    QObject::connect(&argument_parser, &dman::ArgumentParser::openManualWithSearchRequested,
                     &window_manager, &dman::WindowManager::openManualWithSearch);
    // Send openManualRequested() signals after slots connected.
    argument_parser.openManualsDelay();

    // save theme
    DApplicationSettings dApplicationSettings;

    Dtk::Core::DLogManager::registerFileAppender();
    Dtk::Core::DLogManager::registerConsoleAppender();

    // fix error for cutelogger
    // No appenders associated with category js
    auto category = "js";
    auto fileAppender =
        new Dtk::Core::RollingFileAppender(Dtk::Core::DLogManager::getlogFilePath());
    static Dtk::Core::Logger customLoggerInstance(category);
    customLoggerInstance.logToGlobalInstance(category, true);
    customLoggerInstance.registerAppender(fileAppender);


    return app.exec();
}
