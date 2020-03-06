#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QHash>
#include <QList>
#include <QObject>

#include "dbus/dbus_consts.h"
#include "dbus/manual_open_adapter.h"
#include "dbus/manual_open_proxy.h"
#include "dbus/manual_search_adapter.h"
#include "dbus/manual_search_proxy.h"

class windowManager : public QObject
{
    Q_OBJECT
public:
    windowManager(QObject* parent = nullptr);
    bool initDbus();
    bool newWindowOpen(const QString& winId);

private slots:
    void onOpenManual(const QString& appName, const QString& keyName, const QString& titleName);
    void onBindManual(const QString& appName, const QString& winId);
    void onCloseManual(const QString& app_name);

private:
    void initConnect();
    void runShell(const QString& appName, const QString& keyName, const QString& titleName);
    void activeWindow(const QString& winId);

private:
    QString strCurApp;
    ManualOpenProxy* openObj;
    ManualOpenAdapter* openAdapter;
    ManualSearchProxy* searchObj;
    ManualSearchAdapter* searchAdapter;
    QList<QHash<QString, QString>> winInfoList;  // app---winId
};

#endif  // WINDOWMANAGER_H
