/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
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

#ifndef DEEPIN_MANUAL_UI_CHANNEL_SETTINGS_PROXY_H
#define DEEPIN_MANUAL_UI_CHANNEL_SETTINGS_PROXY_H

#include <QObject>
#include <QVariantMap>

namespace dman
{

/**
 * Expose backend settings to web page.
 */
class SettingsProxy : public QObject
{
    Q_OBJECT
public:
    explicit SettingsProxy(QObject *parent = nullptr);

Q_SIGNALS:
    void raiseWindowRequested();
    void fontChangeRequested(const QString &fontFamily, int pixelSize);

public Q_SLOTS:

    /**
     * Request to open url in external web browser.
     * @param url
     */
    void openUrl(const QString &url);


    /**
     * Raise main window.
     */
    void raiseWindow();
};

}  // namespace dman

#endif  // DEEPIN_MANUAL_UI_CHANNEL_SETTINGS_PROXY_H
