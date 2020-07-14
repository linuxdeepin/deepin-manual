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

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H
/*** 2020-06-18 配置文件管理类 10:08:56 wangml ***/
#include <QObject>
#include <QMutexLocker>
#include <QSettings>

class ConfigManager : public QObject
{
    Q_OBJECT
public:
    static ConfigManager *getInstance();

signals:

public slots:

public:
    QString getWinInfoConfigPath();
    QSettings *getSettings();

protected:

private:
    explicit ConfigManager(QObject *parent = 0);
    static ConfigManager *_pInstance;
    static QMutex _mutex;
    QSettings *m_winInfoConfig = nullptr;

};

#endif // CONFIG_MANAGER_H
