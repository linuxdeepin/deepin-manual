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
#include <DApplicationHelper>

#include <QCommandLineParser>
#include <QDBusConnection>

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

} // namespace

ArgumentParser::ArgumentParser(QObject *parent)
    : QObject(parent)
    , bIsDbus(false)
{
}

ArgumentParser::~ArgumentParser()
{
}

/**
 * @brief ArgumentParser::parseArguments 解析帮助手册启动参数,同时注册对外dbus接口
 * @return false: 注册dbus失败,已存在dman,关闭当前进程.  true:注册dbus成功,同时获取当前模块参数.
 */
bool ArgumentParser::parseArguments()
{
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(QCommandLineOption("dbus", "enable daemon mode"));
    parser.process(*(qobject_cast<QGuiApplication *>(this)));

    // 注册Dbus open服务,对外主要接口
    QDBusConnection conn = QDBusConnection::sessionBus();
    ManualOpenProxy *proxy = new ManualOpenProxy(this);
    connect(proxy, &ManualOpenProxy::openManualRequested, this,
            &ArgumentParser::onOpenAppRequested);
    connect(proxy, &ManualOpenProxy::searchRequested, this,
            &ArgumentParser::onSearchRequested);
    ManualOpenAdapter *adapter = new ManualOpenAdapter(proxy);
    Q_UNUSED(adapter);

    //注册Open服务, 如果注册失败,则说明已存在一个dman.
    if (!conn.registerService(kManualOpenService)
            || !conn.registerObject(kManualOpenIface, proxy)) {
        qDebug() << "Failed to register dbus";
        const QStringList position_args = parser.positionalArguments();
        QDBusInterface manual(kManualOpenService, kManualOpenIface, kManualOpenService);
        if (!position_args.isEmpty()) {
            qDebug() << Q_FUNC_INFO << "position_args is not empty";
            for (const QString &arg : position_args) {
                QDBusReply<void> reply = manual.call("Open", arg);
            }
        } else {
            qDebug() << Q_FUNC_INFO << "position_args is empty";
            //激活已有dman
            QDBusReply<void> reply = manual.call("Open", "");
        }
        return false;
    } else {
        qDebug() << "Register dbus service successfully";
        const QStringList position_args = parser.positionalArguments();
        // 不带参为首页,带参跳转到具体模块.
        if (position_args.isEmpty()) {
            //如果通过dbus打开dman, 则parser.isSet("dbus")为true
            if (parser.isSet("dbus")) {
                bIsDbus = true;
            }
        } else {
            curManual = position_args.at(0);
        }
        return true;
    }
}

/**
 * @brief ArgumentParser::openManualsDelay 发送请求信号
 */
void ArgumentParser::openManualsDelay()
{
    qDebug() << Q_FUNC_INFO << curManual;
    if (!bIsDbus) {
        this->onOpenAppRequested(curManual);
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

/**
 * @brief ArgumentParser::onSearchRequested 搜索触发槽
 * @param keyword 搜索关键字
 */
void ArgumentParser::onSearchRequested(const QString &keyword)
{
    qDebug() << Q_FUNC_INFO << keyword;
    emit this->openManualWithSearchRequested("", keyword);
}
