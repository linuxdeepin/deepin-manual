// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <QObject>
#include <QSettings>

class ConfigManager : public QObject
{
    Q_OBJECT
public:
    static ConfigManager *getInstance();
    QString getWinInfoConfigPath();
    QSettings *getSettings();

private:
    explicit ConfigManager(QObject *parent = nullptr);
    static ConfigManager *_pInstance;
    QSettings *m_winInfoConfig = nullptr;
};

#endif // CONFIG_MANAGER_H
