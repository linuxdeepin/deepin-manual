/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp com.deepin.Manual.Search.xml -a manual_search_adapter -c ManualSearchAdapter
 *
 * qdbusxml2cpp is Copyright (C) 2016 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef MANUAL_SEARCH_ADAPTER_H
#define MANUAL_SEARCH_ADAPTER_H

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
 * Adaptor class for interface com.deepin.Manual.Search
 */
class ManualSearchAdapter : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.Manual.Search")
    Q_CLASSINFO("D-Bus Introspection", ""
                                       "  <interface name=\"com.deepin.Manual.Search\">\n"
                                       "    <method name=\"ManualExists\">\n"
                                       "      <arg direction=\"in\" type=\"s\"/>\n"
                                       "      <arg direction=\"out\" type=\"b\"/>\n"
                                       "    </method>\n"
                                       "    <signal name=\"SendWinInfo\">\n"
                                       "      <arg direction=\"out\" type=\"s\" name=\"data\"/>\n"
                                       "    </signal>\n"
                                       "    <method name=\"OnNewWindowOpen\">\n"
                                       "      <arg direction=\"in\" type=\"s\" name=\"data\"/>\n"
                                       "    </method>\n"
                                       "  </interface>\n"
                                       "")
public:
    explicit ManualSearchAdapter(QObject *parent);
    virtual ~ManualSearchAdapter();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    bool ManualExists(const QString &in0);
    void OnNewWindowOpen(const QString &data);

Q_SIGNALS: // SIGNALS
    void SendWinInfo(const QString &data);
};

#endif
