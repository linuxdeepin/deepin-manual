// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MANUAL_OPEN_ADAPTER_H
#define MANUAL_OPEN_ADAPTER_H

#include <QtDBus/QtDBus>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QByteArray;
template<class T>
class QList;
template<class Key, class Value>
class QMap;
class QVariant;
QT_END_NAMESPACE

/*
 * Adaptor class for interface com.deepin.Manual.Open
 */
class ManualOpenAdapter : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.Manual.Open")
    Q_CLASSINFO("D-Bus Introspection",
                ""
                "  <interface name=\"com.deepin.Manual.Open\">\n"
                "    <method name=\"Open\">\n"
                "      <arg direction=\"in\" type=\"s\"/>\n"
                "    </method>\n"
                "    <method name=\"OpenTitle\">\n"
                "      <arg direction=\"in\" type=\"s\"/>\n"
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
    explicit ManualOpenAdapter(QObject *parent = nullptr);
    virtual ~ManualOpenAdapter();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void Open(const QString &in0);
    void OpenTitle(const QString &in0, const QString &in1);
    void Search(const QString &keyword);
    void ShowManual(const QString &in0);
Q_SIGNALS: // SIGNALS
};

#endif
