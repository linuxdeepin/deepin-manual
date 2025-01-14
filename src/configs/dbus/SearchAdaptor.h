// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SEARCHADAPTOR_H
#define SEARCHADAPTOR_H

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
 * Adaptor class for interface com.deepin.Manual.Search
 */
class SearchAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.Manual.Search")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.deepin.Manual.Search\">\n"
"    <method name=\"ManualExists\">\n"
"      <arg direction=\"in\" type=\"s\"/>\n"
"      <arg direction=\"out\" type=\"b\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    SearchAdaptor(QObject *parent);
    virtual ~SearchAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    bool ManualExists(const QString &in0);
Q_SIGNALS: // SIGNALS
};

#endif
