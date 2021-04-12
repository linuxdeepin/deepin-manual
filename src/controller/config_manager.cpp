/*
   * Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
   *
   * Author:     wangmingliang <wangmingliang@uniontech.com>
   *
   * Maintainer: wangmingliang <wangmingliang@uniontech.com>
   *
   * This program is free software: you can redistribute it and/or modify
   * it under the terms of the GNU General Public License as published by
   * the Free Software Foundation, either version 3 of the License, or
   * any later version.
   *
   * This program is distributed in the hope that it will be useful,
   * but WITHOUT ANY WARRANTY; without even the implied warranty of
   * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   * GNU General Public License for more details.
   *
   * You should have received a copy of the GNU General Public License
   * along with this program.  If not, see <http://www.gnu.org/licenses/>.
   */

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
