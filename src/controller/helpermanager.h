#ifndef HELPERMANAGER_H
#define HELPERMANAGER_H

#include <QObject>
#include "controller/filewatcher.h"
#include "controller/search_db.h"

class helperManager  : public QObject
{
    Q_OBJECT
public:
    helperManager(QObject *parent = nullptr);

private:
    void initDbConfig();
    void getModuleInfo();


private:
    fileWatcher *watcherObj;
    SearchDb *dbObj;
};

#endif // HELPERMANAGER_H
