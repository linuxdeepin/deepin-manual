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

#include "base/consts.h"
#include "controller/argument_parser.h"
#include "controller/window_manager.h"
#include "environments.h"
#include "resources/themes/images.h"

#include <DApplication>
#include <DApplicationSettings>
#include <DPlatformWindowHandle>
#include <QDesktopWidget>
#include <QDBusConnection>
#include <QIcon>
#include <DLog>

DWIDGET_USE_NAMESPACE

int main(int argc, char **argv)
{
    qDebug() << __func__ << __LINE__ << QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz");
    qputenv("DXCB_FAKE_PLATFORM_NAME_XCB", "true");
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu");
    //龙芯机器配置,使得DApplication能正确加载QTWEBENGINE
    qputenv("DTK_FORCE_RASTER_WIDGETS", "FALSE");

    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "7777");
    //    Dtk::Widget::DApplication::loadDXcbPlugin();

    Dtk::Widget::DApplication app(argc, argv);
    if (!DPlatformWindowHandle::pluginVersion().isEmpty()) {
        app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);
    }
    //设置窗口属性
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
    //绑定参数解析 信号与槽
    QObject::connect(&argument_parser, &dman::ArgumentParser::newAppOpen,
                     &window_manager, &dman::WindowManager::onNewAppOpen);
    QObject::connect(&argument_parser, &dman::ArgumentParser::openManualWithSearchRequested,
                     &window_manager, &dman::WindowManager::openManualWithSearch);
    QObject::connect(&argument_parser, &dman::ArgumentParser::openManualRequested,
                     &window_manager, &dman::WindowManager::openManual);

    if (!argument_parser.parseArguments()) {
        qDebug() << "argument_parser.parseArguments()";
        //解析参数失败，１００ｍｓ退出进程
        QTimer::singleShot(100, [&]() {
            app.quit();
        });
        return app.exec();
    }
    argument_parser.openManualsDelay();

    // 日志保存, 路径:~/.cach/deepin/deepin-manual/
    DApplicationSettings dApplicationSettings;
    Dtk::Core::DLogManager::registerFileAppender();
    Dtk::Core::DLogManager::registerConsoleAppender();

    // fix error for cutelogger
    // No appenders associated with category js
    // 日志相关
    auto category = "js";
    auto fileAppender =
        new Dtk::Core::RollingFileAppender(Dtk::Core::DLogManager::getlogFilePath());
    static Dtk::Core::Logger customLoggerInstance(category);
    customLoggerInstance.logToGlobalInstance(category, true);
    customLoggerInstance.registerAppender(fileAppender);
    return app.exec();
}
