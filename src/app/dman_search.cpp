// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "dbus/dbus_consts.h"
#include "dbus/manual_search_adapter.h"
#include "dbus/manual_search_proxy.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    ManualSearchProxy search_obj;
    ManualSearchAdapter adapter(&search_obj);

    QDBusConnection conn = QDBusConnection::sessionBus();
    if (!conn.registerService(kManualSearchService)
        || !conn.registerObject(kManualSearchIface, &search_obj)) {
        qCritical() << "dman-search failed to register dbus service";
        return 1;
    } else {
        qDebug() << "dman-search register dbus service success!";
    }

    return app.exec();
}
