#include "config_manager.h"
#include <QDir>
#include <QStandardPaths>

ConfigManager *ConfigManager::_pInstance = 0;
QMutex ConfigManager::_mutex;


ConfigManager::ConfigManager(QObject *parent) : QObject(parent)
{
    m_winInfoConfig = new QSettings(getWinInfoConfigPath(), QSettings::IniFormat);
}

ConfigManager *ConfigManager::getInstance()
{
    if (!_pInstance) {
        QMutexLocker lock(&_mutex);
        if (!_pInstance) {
            ConfigManager *pInstance = new ConfigManager();
            _pInstance = pInstance;
        }
    }
    return _pInstance;
}

QString ConfigManager::getWinInfoConfigPath()
{
    QDir winInfoPath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    if (!winInfoPath.exists()) {
        winInfoPath.mkpath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    }

    QString winInfoFilePath(winInfoPath.filePath("wininfo-config.conf"));

    return winInfoFilePath;
}

QSettings *ConfigManager::getSettings()
{
    return this->m_winInfoConfig;
}
