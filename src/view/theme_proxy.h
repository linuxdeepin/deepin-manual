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

#ifndef THEME_PROXY_H
#define THEME_PROXY_H

#include <dtkgui_global.h>
#include <DGuiApplicationHelper>
#include <QObject>

namespace dman {
class ThemeProxy : public QObject
{
    Q_OBJECT
public:
    explicit ThemeProxy(QObject *parent = nullptr);
    ~ThemeProxy() override;
signals:
    void themeChange(const QString &themetype);
public slots:
    void slot_ThemeChange();
    QString getTheme() const;
};
} // namespace dman
#endif // THEME_PROXY_H
