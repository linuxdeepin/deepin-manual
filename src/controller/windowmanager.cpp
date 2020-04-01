#include "windowmanager.h"
#include <QProcess>

windowManager::windowManager(QObject *parent)
    : QObject(parent)
    //    , openObj(new ManualOpenProxy)
    //    , openAdapter(new ManualOpenAdapter(openObj))
    , searchObj(new ManualSearchProxy)
    , searchAdapter(new ManualSearchAdapter(searchObj))
{
    searchObj->setManagerObj(this);
    initConnect();
}

bool windowManager::initDbus()
{
    //    bool openBool = false;
    //    QDBusConnection conn = QDBusConnection::sessionBus();
    //    if (!conn.registerService(dman::kManualOpenService) ||
    //        !conn.registerObject(dman::kManualOpenIface, openObj)) {
    //        qCritical() << "dman-search failed to register dbus service";
    //    } else {
    //        qDebug() << "dman-search register dbus service success!";
    //        openBool = true;
    //    }

    //    bool searchBool = false;
    QDBusConnection conn1 = QDBusConnection::sessionBus();
    if (!conn1.registerService(dman::kManualSearchService) ||
        !conn1.registerObject(dman::kManualSearchIface, searchObj)) {
        qCritical() << "dman-search failed to register search-dbus service";
        return false;
    } else {
        qDebug() << "dman-search register search-dbus service success!";
        return true;
    }

    //    if (openBool && searchBool) {
    //        return true;
    //    } else {
    //        return false;
    //    }
}

bool windowManager::newWindowOpen(const QString &app_name)
{
    bool bRet = false;
    qDebug() << Q_FUNC_INFO;
    if (winInfoList.count() == 0) {
        bRet = true;
        onBindManual("deepin-manual", app_name);
    } else {
        QHash<QString, QString> winInfo = winInfoList.at(0);
        QString firstWinId = winInfo.values().first();
        bRet = !activeWindow(firstWinId);
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
    qDebug() << Q_FUNC_INFO << appName;
    bool hasProcess = false;
    QString strWinId = "";
    QList<int> removeIndexList;
    for (int i = 0; i < winInfoList.size(); i++) {
        QHash<QString, QString> winInfo = winInfoList.at(i);
        if (winInfo.keys().first() == appName) {
            hasProcess = true;
            strWinId = winInfo.values().first();
            removeIndexList.append(i);
            //            break;
        }
    }

    if (hasProcess) {
        qDebug() << "activewindow..." << strWinId;

        // if active failed,remove that winId-->qhash,then runshell again.
        if (!activeWindow(strWinId)) {
            for (int i = removeIndexList.size() - 1; i >= 0; i--) {
                int removeIndex = removeIndexList.at(i);
                qDebug() << "activeWindow failed,delete it." << removeIndex;
                winInfoList.removeAt(removeIndex);
            }
            runShell(appName, keyName, titleName);
        }
    } else {
        runShell(appName, keyName, titleName);
    }
}

void windowManager::onBindManual(const QString &appName, const QString &winId)
{
    // TODO  if window force to exit, that window can't run closeEvent normanlly,
    //      next new window will create same winId as before.so, we will check it,
    //      and remove same old qhash

    QList<int> removeIndexList;
    for (int i = 0; i < winInfoList.size(); i++) {
        QHash<QString, QString> winInfo = winInfoList.at(i);
        qDebug() << "processId:" << winInfo.keys().first();
        if (winId == winInfo.values().first()) {
            removeIndexList.append(i);
        }
    }

    if (removeIndexList.size() > 0) {
        for (int i = removeIndexList.size() - 1; i >= 0; i--) {
            int removeIndex = removeIndexList.at(i);
            qDebug() << "exist same winId ,delete it." << removeIndex;
            winInfoList.removeAt(removeIndex);
        }
    }

    qDebug() << Q_FUNC_INFO << appName << winId;
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

    if (winInfoList.count() <= 0) {
        // if dman is empty,quit dman-search;
        qApp->quit();
    }
}

void windowManager::initConnect()
{
//    connect(openObj, &ManualOpenProxy::openManual, this, &windowManager::onOpenManual);
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

    QProcess::startDetached("dman", argList);
    //    QProcess process;
    //    process.start("/usr/bin/dman", argList);
    //    process.waitForStarted();
    //    process.waitForFinished();
    //    process.start("/home/archermind/Desktop/dman", argList);
    //    process.start("/home/archermind/Documents/gitWork/build-manual-qt5_11_3-Debug/src/dman",
    //                  argList);
    //    process.waitForBytesWritten();
    //    process.waitForFinished(1);
}

bool windowManager::activeWindow(const QString &winId)
{
    bool bRet = false;
    QDBusInterface manual("com.deepin.dde.daemon.Dock", "/com/deepin/dde/daemon/Dock",
                          "com.deepin.dde.daemon.Dock");
    quintptr nwinId = winId.toULong();
    QDBusReply<void> reply = manual.call("ActivateWindow", nwinId);
    if (reply.isValid()) {
        qDebug() << "call com.deepin.dde.daemon.Dock success";
        bRet = true;
    } else {
        qDebug() << "call com.deepin.dde.daemon.Dock failed" << reply.error();
    }
    return bRet;
}
