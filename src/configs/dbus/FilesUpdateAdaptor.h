// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FILESUPDATEADAPTOR_H
#define FILESUPDATEADAPTOR_H

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
 * Adaptor class for interface local.ManualFilesUpdateProxy
 */
class FilesUpdateAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "local.ManualFilesUpdateProxy")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"local.ManualFilesUpdateProxy\">\n"
"    <method name=\"OnFilesUpdate\">\n"
"      <arg direction=\"in\" type=\"as\" name=\"list\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    FilesUpdateAdaptor(QObject *parent);
    virtual ~FilesUpdateAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void OnFilesUpdate(const QStringList &list);
Q_SIGNALS: // SIGNALS
};

#endif
