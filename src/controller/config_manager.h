#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H
/*** 2020-06-18 配置文件管理类 10:08:56 wangml ***/
#include <QObject>
#include <QMutexLocker>
#include <QSettings>

class ConfigManager : public QObject
{
    Q_OBJECT
public:
    static ConfigManager *getInstance();
    QString getWinInfoConfigPath();
    QSettings *getSettings();

signals:

public slots:

protected:

private:
    explicit ConfigManager(QObject *parent = 0);
    static ConfigManager *_pInstance;
    static QMutex _mutex;
    QSettings *m_winInfoConfig = nullptr;

};

#endif // CONFIG_MANAGER_H
