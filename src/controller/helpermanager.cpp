#include "helpermanager.h"
#include "base/command.h"




helperManager::helperManager(QObject *parent)
    : QObject(parent)
    ,watcherObj(new fileWatcher)
    , dbObj(new SearchDb)
{
    initDbConfig();
    getModuleInfo();
}




void helperManager::initDbConfig()
{
    QString  dbPath = Utils::getSystemManualDir() + "/search.db";
    dbObj->initDb(dbPath);
    dbObj->initTimeTable();
}

void helperManager::getModuleInfo()
{

}


