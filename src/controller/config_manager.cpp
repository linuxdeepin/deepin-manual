// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "config_manager.h"
#include "base/ddlog.h"

#include <QDir>
#include <QStandardPaths>
#include <QtDebug>

ConfigManager *ConfigManager::_pInstance = nullptr;

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent)
{
    qCDebug(app) << "Creating ConfigManager instance";
    QString configPath = getWinInfoConfigPath();
    qCDebug(app) << "Loading config from:" << configPath;
    m_winInfoConfig = new QSettings(configPath, QSettings::IniFormat, this);
}

ConfigManager::~ConfigManager()
{
}



/**
 * @brief ConfigManager::getInstance
 * @return 返回类实例
 * 获取单例指针
 */
ConfigManager *ConfigManager::getInstance()
{
    if (!_pInstance) {
        qCDebug(app) << "Creating new ConfigManager instance";
        _pInstance =  new ConfigManager();
    }
    return _pInstance;
}

void ConfigManager::releaseInstance()
{
    if (_pInstance) {
        qCDebug(app) << "Releasing ConfigManager instance";
        delete _pInstance;
    }

    _pInstance = nullptr;
}

/**
 * @brief ConfigManager::getWinInfoConfigPath
 * @return 返回配置文件目录
 * 获取配置文件目录
 */
QString ConfigManager::getWinInfoConfigPath()
{
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    qCDebug(app) << "Config directory:" << configDir;
    
    QDir winInfoPath(configDir);
    if (!winInfoPath.exists()) {
        winInfoPath.mkpath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    }

    QString winInfoFilePath(winInfoPath.filePath("wininfo-config.conf"));
    qCDebug(app) << "Config file path:" << winInfoFilePath;

    return winInfoFilePath;
}

/**
 * @brief ConfigManager::getSettings
 * @return 返回私有成员指针
 * 返回QSettings指针，对文件进行操作
 */
QSettings *ConfigManager::getSettings()
{
    return this->m_winInfoConfig;
}
