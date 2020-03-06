#include "windowmanager.h"
#include <QProcess>

windowManager::windowManager(QObject *parent)
    : QObject(parent)
    , openObj(new ManualOpenProxy)
    , openAdapter(new ManualOpenAdapter(openObj))
    , searchObj(new ManualSearchProxy)
    , searchAdapter(new ManualSearchAdapter(searchObj))
{
    searchObj->setManagerObj(this);
    initConnect();
}

bool windowManager::initDbus()
{
    bool openBool = false;
    QDBusConnection conn = QDBusConnection::sessionBus();
    if (!conn.registerService(dman::kManualOpenService) ||
        !conn.registerObject(dman::kManualOpenIface, openObj)) {
        qCritical() << "dman-search failed to register dbus service";
    } else {
        qDebug() << "dman-search register dbus service success!";
        openBool = true;
    }

    bool searchBool = false;
    QDBusConnection conn1 = QDBusConnection::sessionBus();
    if (!conn1.registerService(dman::kManualSearchService) ||
        !conn1.registerObject(dman::kManualSearchIface, searchObj)) {
        qCritical() << "dman-search failed to register search-dbus service";
    } else {
        qDebug() << "dman-search register search-dbus service success!";
        searchBool = true;
    }

    if (openBool && searchBool) {
        return true;
    } else {
        return false;
    }
}

bool windowManager::newWindowOpen(const QString &winId)
{
    bool bRet = false;
    qDebug() << Q_FUNC_INFO;
    if (winInfoList.count() == 0) {
        bRet = true;
        qDebug() << "new manual";
        onBindManual("deepin-manual", winId);
    } else {
        qDebug() << "old manual//";
        QHash<QString, QString> winInfo = winInfoList.at(0);
        QString winId = winInfo.values().first();
        activeWindow(winId);
    }
    return bRet;
}

/**
 * @brief windowManager::onOpenManual
 * @param appName
 * @param keyName
 * @param titleName
 */
void windowManager::onOpenManual(const QString &appName, const QString &keyName,
                                 const QString &titleName)
{
    qDebug() << Q_FUNC_INFO;
    bool hasProcess = false;
    QString strWinId = "";
    for (int i = 0; i < winInfoList.size(); i++) {
        QHash<QString, QString> winInfo = winInfoList.at(i);
        if (winInfo.keys().first() == appName) {
            hasProcess = true;
            strWinId = winInfo.values().first();
            break;
        }
    }

    if (hasProcess) {
        qDebug() << "activewindow...";
        activeWindow(strWinId);
    } else {
        qDebug() << "runShell...";
        runShell(appName, keyName, titleName);
    }
    qDebug() << Q_FUNC_INFO << winInfoList.size();
}

void windowManager::onBindManual(const QString &appName, const QString &winId)
{
    QHash<QString, QString> winInfo;
    winInfo.insert(appName, winId);
    winInfoList.append(winInfo);
}

void windowManager::onCloseManual(const QString &app_name)
{
    QString strtep = app_name;
    if (strtep.isEmpty()) {
        strtep = "deepin-manual";
    }
    int removeWinIndex = -1;
    if (winInfoList.size() > 0) {
        for (int i = 0; i < winInfoList.size(); i++) {
            QHash<QString, QString> winInfo = winInfoList.at(i);
            if (strtep == winInfo.key(winInfo.values().first())) {
                removeWinIndex = i;
                qDebug() << "remove window" << removeWinIndex;
                break;
            }
        }

        if (removeWinIndex != -1) {
            winInfoList.removeAt(removeWinIndex);
        }
    }
    qDebug() << Q_FUNC_INFO << winInfoList.size();
}

void windowManager::initConnect()
{
    connect(openObj, &ManualOpenProxy::openManual, this, &windowManager::onOpenManual);
    connect(searchObj, &ManualSearchProxy::bindManual, this, &windowManager::onBindManual);
    connect(searchObj, &ManualSearchProxy::closeManual, this, &windowManager::onCloseManual);
}

void windowManager::runShell(const QString &appName, const QString &keyName,
                             const QString &titleName)
{
    qDebug() << Q_FUNC_INFO;
    // run shell
    QStringList argList;
    QString strArgu = appName + "%" + keyName + "%" + titleName;
    qDebug() << strArgu;
    argList << strArgu;
    QProcess process;
    process.start("/usr/bin/dman", argList);
    //    process.waitForStarted();
    //    process.waitForFinished();
    //    process.start("/home/archermind/Desktop/dman", argList);
    //    process.start("/home/archermind/Documents/gitWork/build-manual-qt5_11_3-Debug/src/dman",
    //                  argList);
    process.waitForBytesWritten();
    process.waitForFinished(1);
}

void windowManager::activeWindow(const QString &winId)
{
    QDBusInterface manual("com.deepin.dde.daemon.Dock", "/com/deepin/dde/daemon/Dock",
                          "com.deepin.dde.daemon.Dock");
    quintptr nwinId = winId.toULong();
    QDBusReply<void> reply = manual.call("ActivateWindow", nwinId);
    if (reply.isValid()) {
        qDebug() << "call com.deepin.dde.daemon.Dock success";
    } else {
        qDebug() << "call com.deepin.dde.daemon.Dock failed" << reply.error();
    }
}
