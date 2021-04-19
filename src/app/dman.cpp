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
#include "controller/shellobj.h"

#include <DApplication>
#include <DApplicationSettings>
#include <DPlatformWindowHandle>
#include <QDesktopWidget>
#include <QDBusConnection>
#include <QIcon>
#include <DLog>
#include <QLocale>
DWIDGET_USE_NAMESPACE

int main(int argc, char **argv)
{
    QDateTime time;
    qputenv("DXCB_FAKE_PLATFORM_NAME_XCB", "true");
    //禁用GPU
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu");
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--single-process");
    //所有进程类型禁用沙箱..此配置开启禁用gpu后无效
//    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--no-sandbox");
    //龙芯机器配置,使得DApplication能正确加载QTWEBENGINE
    qputenv("DTK_FORCE_RASTER_WIDGETS", "FALSE");
    //访问http://127.0.0.1:7777/调试
    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "7777");
    //    Dtk::Widget::DApplication::loadDXcbPlugin();

    Dtk::Widget::DApplication app(argc, argv);
    if (!DPlatformWindowHandle::pluginVersion().isEmpty()) {
        app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);
    }
    qInfo() << QString(QLocale::system().name()) << QLocale().name();
#ifdef DCPU_IS_LONGSON
    //add by wujian 20200907 for 解决龙芯平台，QWebEngine因字体库字体太多，造成启动失败的问题
    QString strHomePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString strExeShell = QString("rm -fr %1/.cache/fontconfig").arg(strHomePath);
    shellObj::execSystem(strExeShell);
#endif

    //设置窗口属性
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    app.setWindowIcon(QIcon::fromTheme("deepin-manual"));
    app.setProductIcon(QIcon::fromTheme("deepin-manual"));
    app.setOrganizationName("deepin");
    app.setOrganizationDomain("deepin.org");
    app.setApplicationVersion(VERSION);
    app.setApplicationName(kAppName);
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
    qInfo() << Dtk::Core::DLogManager::getlogFilePath() << fileAppender->datePatternString();
    static Dtk::Core::Logger customLoggerInstance(category);
    customLoggerInstance.logToGlobalInstance(category, true);
    customLoggerInstance.registerAppender(fileAppender);

    return app.exec();
}
