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

#include "dbus/dbusvariant/app_info.h"

AppInfo::AppInfo()
    : desktop(),
      name(),
      key(),
      icon_key(),
      category_id(-1),
      installed_time(0) {

}

AppInfo::~AppInfo() {

}

// static
void AppInfo::registerMetaType() {
  qRegisterMetaType<AppInfo>("AppInfo");
  qDBusRegisterMetaType<AppInfo>();
  qRegisterMetaType<AppInfoList>("AppInfoList");
  qDBusRegisterMetaType<AppInfoList>();
}

QDebug operator<<(QDebug debug, const AppInfo& info) {
  debug << info.category_id
        << info.installed_time
        << info.desktop
        << info.name
        << info.key
        << info.icon_key;
  return debug;
}

QDBusArgument& operator<<(QDBusArgument& argument,
                          const AppInfo& info) {
  argument.beginStructure();
  argument << info.desktop
           << info.name
           << info.key
           << info.icon_key
           << info.category_id
           << info.installed_time;
  argument.endStructure();
  return argument;
}

QDataStream& operator<<(QDataStream& stream, const AppInfo& info) {
  stream << info.desktop
         << info.name
         << info.key
         << info.icon_key
         << info.category_id
         << info.installed_time;
  return stream;
}

const QDBusArgument& operator>>(const QDBusArgument& argument,
                                AppInfo& info) {
  argument.beginStructure();
  argument >> info.desktop
           >> info.name
           >> info.key
           >> info.icon_key
           >> info.category_id
           >> info.installed_time;
  argument.endStructure();
  return argument;
}

const QDataStream& operator>>(QDataStream& stream, AppInfo& info) {
  stream >> info.desktop
         >> info.name
         >> info.key
         >> info.icon_key
         >> info.category_id
         >> info.installed_time;
  return stream;
}