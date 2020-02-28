#include "windowmanager.h"
#include <QProcess>

windowManager::windowManager(QObject *parent)
    : QObject(parent)
    , openObj(new ManualOpenProxy)
    , openAdapter(new ManualOpenAdapter(openObj))
    , searchObj(new ManualSearchProxy)
    , searchAdapter(new ManualSearchAdapter(searchObj))
{
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
    //    return true;

    if (openBool && searchBool) {
        return true;
    } else {
        return false;
    }
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
    // run shell
    QStringList argList;
    QString strArgu = appName + "%" + keyName + "%" + titleName;
    argList << strArgu;
    QProcess process;
    process.start("/usr/bin/dman", argList);
    process.waitForBytesWritten();
    process.waitForFinished(1);
}

void windowManager::onBindManual(const QString &appName, const QString &winId)
{
    QHash<QString, QString> winInfo;
    winInfo.insert(appName, winId);
    winInfoList.append(winInfo);
}

void windowManager::onCloseManual(const QString &winId)
{
    int removeWinIndex = -1;
    if (winInfoList.size() > 0) {
        for (int i = 0; i < winInfoList.size(); i++) {
            QHash<QString, QString> winInfo = winInfoList.at(i);
            if (winId == winInfo.value(winInfo.keys().first())) {
                removeWinIndex = i;
                qDebug() << "remove window" << removeWinIndex;
                break;
            }
        }

        if (removeWinIndex != -1) {
            winInfoList.removeAt(removeWinIndex);
        }
    }
}

void windowManager::initConnect()
{
    connect(openObj, &ManualOpenProxy::openManual, this, &windowManager::onOpenManual);
    connect(searchObj, &ManualSearchProxy::bindManual, this, &windowManager::onBindManual);
    connect(searchObj, &ManualSearchProxy::closeManual, this, &windowManager::onCloseManual);
}
