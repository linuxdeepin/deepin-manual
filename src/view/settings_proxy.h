// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEEPIN_MANUAL_UI_CHANNEL_SETTINGS_PROXY_H
#define DEEPIN_MANUAL_UI_CHANNEL_SETTINGS_PROXY_H

#include <QVariantMap>

/**
 * @brief The SettingsProxy class
 * 页面设置接口类
 */
class SettingsProxy : public QObject
{
    Q_OBJECT
public:
    explicit SettingsProxy(QObject *parent = nullptr);

Q_SIGNALS:
    void fontChangeRequested(const QString &fontFamily, int pixelSize);

};

#endif // DEEPIN_MANUAL_UI_CHANNEL_SETTINGS_PROXY_H
