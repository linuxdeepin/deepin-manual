// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEEPIN_MANUAL_DBUS_DBUS_VARIANT_APP_INFO_H
#define DEEPIN_MANUAL_DBUS_DBUS_VARIANT_APP_INFO_H

#include <QtDBus>

struct AppInfo {
public:
    AppInfo();
    ~AppInfo();

    static void registerMetaType();
    inline bool operator==(const AppInfo &other) const
    {
        return desktop == other.desktop;
    }

    friend QDebug operator<<(QDebug debug, const AppInfo &info);
    friend QDBusArgument &operator<<(QDBusArgument &argument,
                                     const AppInfo &info);
    friend QDataStream &operator<<(QDataStream &stream, const AppInfo &info);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument,
                                           AppInfo &info);
    friend const QDataStream &operator>>(QDataStream &stream, AppInfo &info);

    QString desktop;
    QString name;
    QString key;
    QString icon_key;
    qlonglong category_id;
    qlonglong installed_time;
};

typedef QList<AppInfo> AppInfoList;

Q_DECLARE_METATYPE(AppInfo)

Q_DECLARE_METATYPE(AppInfoList)

#endif // DEEPIN_MANUAL_DBUS_DBUS_VARIANT_APP_INFO_H
