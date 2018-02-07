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

#include "view/manual_proxy.h"

#include <QDBusConnection>
#include <QDesktopServices>

#include "dbus/dbus_consts.h"
#include "dbus/launcher_interface.h"

namespace dman {

ManualProxy::ManualProxy(QObject* parent)
    : QObject(parent),
      launcher_interface_(new LauncherInterface(
          kLauncherService,
          kLauncherIface,
          QDBusConnection::sessionBus(),
          this)) {
  AppInfo::registerMetaType();
}

ManualProxy::~ManualProxy() {

}

QStringList ManualProxy::getSystemManualList() {
  const QHash<QString, QString> kAppNameMap = {
      { "com.deepin.Calendar", "dde-calendar" },
      { "com.deepin.Music", "deepin-music" },
      { "com.deepin.Screenshot", "deepin-screenshot" },
      { "com.deepin.VoiceRecorder", "deepin-voice-recorder" },
      { "deepin-cloud-print-configurator", "deepin-cloud-print" },
      { "com.deepin.ImageViewer", "deepin-image-viewer" },
      { "deepin-cloud-scan-configurator", "deepin-cloud-scan" },
      { "com.deepin.Movie", "deepin-movie" },
      { "com.deepin.ScreenRecorder", "deepin-screen-recorder" },
  };

  if (app_list_.isEmpty()) {
    const QStringList dir_entry = QDir(this->getSystemManualDir()).entryList();
    const AppInfoList list = launcher_interface_->GetAllItemInfos();
    for (const AppInfo& info : list) {
      const QString app_name = kAppNameMap.value(info.key, info.key);
      if ((dir_entry.indexOf(app_name) != -1) &&
          app_list_.indexOf(app_name) == -1) {
        app_list_.append(app_name);
      }
    }

    // Add "dde" by hand, as it has no desktop file.
    app_list_.append("dde");

    // Remove youdao-dict if current locale is not Simplified Chinese.
    if (!QLocale().name().startsWith("zh")) {
      app_list_.removeAll("youdao-dict");
    }
  }
  qDebug() << "app list:" << app_list_;
  return app_list_;
}

void ManualProxy::openExternalLink(const QString& url) {
  QDesktopServices::openUrl(url);
}

}  // namespace dman