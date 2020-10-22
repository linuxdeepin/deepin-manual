#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include "base/utils.h"

#include <QObject>
#include <QFileSystemWatcher>

class fileWatcher : public QObject
{
    Q_OBJECT
public:
    fileWatcher(QObject *parent = nullptr);

private:
    void monitorFile();

private slots:
    void onChangeFile(const QString &);
    void onChangeDirSlot(const QString &);

private:
    QFileSystemWatcher *watcherObj;
    QStringList listMonitorFile;
    QStringList listModule;
};

#endif // FILEWATCHER_H
