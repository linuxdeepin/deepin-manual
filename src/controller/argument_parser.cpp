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

#include "controller/argument_parser.h"

#include "base/consts.h"
#include "base/utils.h"
#include "dbus/dbus_consts.h"
#include "dbus/manual_open_adapter.h"
#include "dbus/manual_open_interface.h"
#include "dbus/manual_open_proxy.h"
#include "view/web_window.h"
#include "window_manager.h"

#include <DLog>
#include <QCommandLineParser>
#include <QDBusConnection>

#include <DApplicationHelper>

namespace dman {

namespace {

QString ConvertOldDmanPath(const QString &app_name)
{
    const QStringList parts = app_name.split('/');
    const int dman_index = parts.indexOf("dman");
    if (dman_index > 0 && dman_index < parts.length() - 1) {
        return parts.at(dman_index + 1);
    }
    return app_name;
}

}  // namespace

ArgumentParser::ArgumentParser(QObject *parent)
    : QObject(parent)
{
}

ArgumentParser::~ArgumentParser() {}

bool ArgumentParser::parseArguments()
{
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(QCommandLineOption("dbus", "enable daemon mode"));
    parser.parse(qApp->arguments());

    // Register dbus service.
    QDBusConnection conn = QDBusConnection::sessionBus();
    ManualOpenProxy *proxy = new ManualOpenProxy(this);
    connect(proxy, &ManualOpenProxy::openManualRequested, this,
            &ArgumentParser::onOpenAppRequested);
    connect(proxy, &ManualOpenProxy::searchRequested, this, &ArgumentParser::onSearchRequested);

    ManualOpenAdapter *adapter = new ManualOpenAdapter(proxy);
    Q_UNUSED(adapter);

    if (!conn.registerService(kManualOpenService) ||
            !conn.registerObject(kManualOpenIface, proxy)) {
        qDebug() << "Failed to register dbus";

        // Failed to register dbus service.
        // Open appName list with dbus interface.
        const QStringList position_args = parser.positionalArguments();
        qDebug() << "position_args:" << position_args;

        if (!position_args.isEmpty()) {
            qDebug() << "position_args is not empty";

            if (position_args.size() > 1) {
                qDebug() << "position_args.size() > 1:" << position_args.size();
            }

            ManualOpenInterface *iface = new ManualOpenInterface(
                kManualOpenService, kManualOpenIface, QDBusConnection::sessionBus(), this);

            if (iface->isValid()) {
                // Call dbus interface.
                // Only parse positional arguments.
                for (const QString &arg : position_args) {
                    iface->Open(arg);
                }
            } else {
                // It may fail to register dbus service in root session.
                // Create new instance.
                for (const QString &manual : position_args) {
                    manuals_.append(manual);
                }
                return false;
            }
        } else {
            qDebug() << "position_args is empty";

            QString argName = qApp->arguments().value(0);
            qDebug() << "argName:" << argName;

            if (argName == QString(kAppProcessName) ||
                    argName.endsWith("/" + QString(kAppProcessName))) {
                qDebug() << "emit onNewAppOpen";
                emit onNewAppOpen();
            }
        }

        return true;
    } else {
        qDebug() << "Register dbus service successfully";
        const QStringList position_args = parser.positionalArguments();
        if (position_args.isEmpty()) {
            // Open index page if not in dbus daemon mode.
            if (!parser.isSet("dbus")) {
                manuals_.append("");
            }
        } else {
            // Only parse positional arguments.
            for (const QString &manual : position_args) {
                manuals_.append(manual);
            }
        }

        return false;
    }
}

void ArgumentParser::openManualsDelay()
{
    qDebug() << "call openManualsDelay";
    for (const QString &manual : manuals_) {
        qDebug() << Q_FUNC_INFO << manual;
        this->onOpenAppRequested(manual);
    }
}

/**
 * @brief ArgumentParser::onOpenAppRequested 打开对应模块帮助手册
 * @param app_name   模块名称
 * @param title_name 需要定位到的标题名称,如果为空,则定位在概述.
 */
void ArgumentParser::onOpenAppRequested(const QString &app_name, const QString &title_name)
{
    const QString compact_app_name = ConvertOldDmanPath(app_name);
    //通过语言映射表,将传入标题名称映射转换成对应名称.
    const QString title = Utils::translateTitle(title_name);
    qDebug() << Q_FUNC_INFO << compact_app_name << "---" << title;
    emit this->openManualRequested(compact_app_name, title);
}

void ArgumentParser::onSearchRequested(const QString &keyword)
{
    qDebug() << Q_FUNC_INFO << keyword;
    emit this->openManualWithSearchRequested("", keyword);
}

}  // namespace dman
