#include "ut_argument_parser.h"

#include "controller/argument_parser.h"
#include "dbus/dbus_consts.h"
#include "dbus/manual_open_proxy.h"
#include "dbus/manual_open_adapter.h"

#include "QCommandLineParser"
#include <QDBusInterface>
#include "../third-party/stub/stub.h"
ut_argument_parser_test::ut_argument_parser_test()
{

}

bool stubregisterServicefalse()
{
    return  false;
}


TEST_F(ut_argument_parser_test, parseArguments)
{

    QProcess p;
    p.start("dman deepin-app-store");
    p.close();
    ArgumentParser ap;
    qDebug() << "pars eArguments.bool-->" << ap.parseArguments();

    Stub st;
    st.set((bool (QDBusConnection::*)(const QString &))ADDR(QDBusConnection, registerService), stubregisterServicefalse);
    ap.parseArguments();

    st.set((bool (QStringList::*)() const)ADDR(QStringList, isEmpty), stubregisterServicefalse);
    ap.parseArguments();
}

TEST_F(ut_argument_parser_test, parseArguments2)
{
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(QCommandLineOption("dbus", "enable daemon mode"));
    QStringList list;
    list << "deepin-app-store";
    parser.parse(list);

    // 注册Dbus open服务,对外主要接口
    QDBusConnection conn = QDBusConnection::sessionBus();
    conn.registerService(kManualOpenService);

    ArgumentParser ap;
    qDebug() << "pars eArguments.bool-->" << ap.parseArguments();
}

TEST_F(ut_argument_parser_test, parseArguments3)
{
    ArgumentParser ap;
    qDebug() << "pars eArguments.bool-->" << ap.parseArguments();

}

TEST_F(ut_argument_parser_test, openManualsDelay)
{
    ArgumentParser *ap = new ArgumentParser;
    ap->openManualsDelay();
    delete ap;
}

TEST_F(ut_argument_parser_test, onSearchRequested)
{
    ArgumentParser ap;
    ap.onSearchRequested("应用");
}

TEST_F(ut_argument_parser_test, onOpenAppRequested)
{
    ArgumentParser ap;
    ap.onOpenAppRequested("应用", "打开");
}
