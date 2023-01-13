#include "ut_search_manager.h"
#include "base/utils.h"
#include "controller/search_manager.h"
#include "src/third-party/stub/stub.h"
#include "controller/search_db.h"

#include <QStandardPaths>
#include <QSignalSpy>

ut_search_manager_test::ut_search_manager_test()
{
}

QString ut_search_manager_test::stub_writableLocation()
{
    return "/tmp";
}

QString search_managerfirstremovedbfile()
{
    QString dbdir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append("/.local/share/deepin/deepin-manual");
    QString databasePath1 = dbdir.append("/search.db");
    qWarning() << "db file:" << databasePath1;
    QFile::remove(databasePath1);

    QFile file(databasePath1);

    if (!file.exists()) {
        qWarning() << "delete file success";
    }

    return databasePath1;
}

void stub_initSearchManager()
{
    return;
}

TEST_F(ut_search_manager_test, initSearchManager)
{
    Stub s;
    s.set(ADDR(QStandardPaths, writableLocation), ADDR(ut_search_manager_test, stub_writableLocation));
    QString databasePath1 = search_managerfirstremovedbfile();
    SearchManager sm;
}
