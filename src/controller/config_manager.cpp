// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "config_manager.h"

#include <QDir>
#include <QStandardPaths>

ConfigManager *ConfigManager::_pInstance = nullptr;

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent)
{
    m_winInfoConfig = new QSettings(getWinInfoConfigPath(), QSettings::IniFormat, this);
}



/**
 * @brief ConfigManager::getInstance
 * @return 返回类实例
 * 获取单例指针
 */
ConfigManager *ConfigManager::getInstance()
{
    if (!_pInstance) {
        _pInstance =  new ConfigManager();
    }
    return _pInstance;
}

/**
 * @brief ConfigManager::getWinInfoConfigPath
 * @return 返回配置文件目录
 * 获取配置文件目录
 */
QString ConfigManager::getWinInfoConfigPath()
{
    QDir winInfoPath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    if (!winInfoPath.exists()) {
        winInfoPath.mkpath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    }

    QString winInfoFilePath(winInfoPath.filePath("wininfo-config.conf"));

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
