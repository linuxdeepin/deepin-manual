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

#include <QCoreApplication>
#include <QDBusConnection>
#include <DLog>

#include "dbus/dbus_consts.h"
#include "dbus/manual_search_adapter.h"
#include "dbus/manual_search_proxy.h"

int main(int argc, char** argv) {
  QCoreApplication app(argc, argv);

  ManualSearchProxy search_obj;
  ManualSearchAdapter adapter(&search_obj);

  QDBusConnection conn = QDBusConnection::sessionBus();
  if (!conn.registerService(dman::kManualSearchService) ||
      !conn.registerObject(dman::kManualSearchIface, &search_obj)) {
        qCritical() << "dman-search failed to register dbus service";
        return 1;
  }
  else {
      qDebug() << "dman-search register dbus service success!";
  }

  return app.exec();
}
