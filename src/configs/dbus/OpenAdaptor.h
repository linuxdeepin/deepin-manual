// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OPENADAPTOR_H
#define OPENADAPTOR_H

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
 * Adaptor class for interface com.deepin.Manual.Open
 */
class OpenAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.Manual.Open")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.deepin.Manual.Open\">\n"
"    <method name=\"Open\">\n"
"      <arg direction=\"in\" type=\"s\"/>\n"
"    </method>\n"
"    <method name=\"ShowManual\">\n"
"      <arg direction=\"in\" type=\"s\"/>\n"
"    </method>\n"
"    <method name=\"Search\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"keyword\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    OpenAdaptor(QObject *parent);
    virtual ~OpenAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void Open(const QString &in0);
    void Search(const QString &keyword);
    void ShowManual(const QString &in0);
Q_SIGNALS: // SIGNALS
};

#endif
