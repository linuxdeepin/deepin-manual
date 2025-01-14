// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LAUNCHER1ADAPTOR_H
#define LAUNCHER1ADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

/*
 * Adaptor class for interface org.deepin.dde.daemon.Launcher1
 */
class Launcher1Adaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.deepin.dde.daemon.Launcher1")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"org.deepin.dde.daemon.Launcher1\">\n"
"    <method name=\"GetAllItemInfos\">\n"
"      <arg direction=\"out\" type=\"a(ssssxx)\"/>\n"
"      <annotation value=\"AppInfoList\" name=\"org.qtproject.QtDBus.QtTypeName.Out0\"/>\n"
"    </method>\n"
"    <method name=\"GetItemInfo\">\n"
"      <arg direction=\"in\" type=\"s\"/>\n"
"      <arg direction=\"out\" type=\"(ssssxx)\"/>\n"
"      <annotation value=\"AppInfo\" name=\"org.qtproject.QtDBus.QtTypeName.Out0\"/>\n"
"    </method>\n"
"    <signal name=\"ItemChanged\">\n"
"      <arg type=\"s\"/>\n"
"      <arg type=\"(ssssxx)\"/>\n"
"      <arg type=\"x\"/>\n"
"      <annotation value=\"AppInfo\" name=\"org.qtproject.QtDBus.QtTypeName.In1\"/>\n"
"    </signal>\n"
"  </interface>\n"
        "")
public:
    Launcher1Adaptor(QObject *parent);
    virtual ~Launcher1Adaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    AppInfoList GetAllItemInfos();
    AppInfo GetItemInfo(const QString &in0);
Q_SIGNALS: // SIGNALS
    void ItemChanged(const QString &in0, AppInfo in1, qlonglong in2);
};

#endif
