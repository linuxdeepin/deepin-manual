#include "helpermanager.h"
#include "base/command.h"
#include <QDBusMessage>
#include <QtDBus>
#include <QDBusConnection>

helperManager::helperManager(QObject *parent)
    : QObject(parent)
    , watcherObj(new fileWatcher)
    , dbObj(new SearchDb)
{
    initDbConfig();
    getModuleInfo();
    initConnect();
}

void helperManager::initDbConfig()
{
    dbObj->initDb();
    dbObj->initSearchTable();
    dbObj->initTimeTable();
}

void helperManager::getModuleInfo()
{
    //获取数据库中所有文件更新时间
    QMap<QString, QString> mapFile =  dbObj->selectAllFileTime();
    QMap<QString, QString> mapNow;
    QString  assetsPath = Utils::getSystemManualDir();

    for (const QString &type : QDir(assetsPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
        if (type == "system" || type == "application") {
            QString typePath = assetsPath + "/" + type;
            //监控资源文件夹
            for (QString &module : QDir(typePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                QString modulePath = typePath + "/" + module;
                QStringList listLang;
                for (QString &lang : QDir(modulePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                    if (lang == "zh_CN" || lang == "en_US") {
                        listLang.append(lang);
                        QString strMd = modulePath + "/" + lang + "/index.md";
                        QFileInfo fileInfo(strMd);
                        if (fileInfo.exists()) {
                            QString modifyTime = fileInfo.lastModified().toString();
                            mapNow.insert(strMd, modifyTime);
                        }
                    }
                }
            }

        }
    }

    QStringList deleteList;
    QStringList addList;
    QStringList addTime;
    watcherObj->setFileMap(mapNow);
    //白名单对比，得到差异列表信息
    watcherObj->checkMap(mapFile, mapNow, deleteList, addList, addTime);
    handleDb(deleteList, addList, addTime);
}

void helperManager::initConnect()
{
    connect(watcherObj, &fileWatcher::filelistChange, this, &helperManager::onFilelistChange);
}

/**
 * @brief helperManager::handleDb
 * @param deleteList   比较得出删除的文件
 * @param addList      比较得出新增的文件（文件增加 & 文件被修改）
 * @param addTime      新增文件更新时间
 */
void helperManager::handleDb(const QStringList &deleteList, const QStringList &addList, const QStringList &addTime)
{
    qDebug() << "========>" << deleteList.count() << " " << addList.count();
    dbusSend(deleteList, addList);
    if (!deleteList.isEmpty()) {
        dbObj->deleteFilesTimeEntry(deleteList);
        QStringList appList;
        QStringList langList;
        for (const QString &path : deleteList) {
            QStringList list = path.split("/");
            if (list.count() > 2) {
                langList.append(list.at(list.count() - 2));
                appList.append(list.at(list.count() - 3));
            }
        }
        dbObj->deleteSearchInfo(appList, langList);
    }

    if (!addList.isEmpty() && !addTime.isEmpty()) {
        dbObj->insertFilesTimeEntry(addList, addTime);


        QString out, err;
        QStringList cmd;
        cmd.append(QString("%1/toSearch/toSearchIndex.js").arg(DMAN_WEB_DIR));
        cmd.append(addList);
        const bool ok = dman::SpawnCmd("/usr/bin/node", cmd, out, err);
        qDebug() << "===========>" << ok;

        QStringList outList = out.split("\n");
        for (int i = 0; i < outList.count() - 1; i++) {
            const QString &outKey = outList.at(i);
            QStringList pathList = addList.at(i).split("/");
            if (pathList.count() < 3) return;
            const QString &lang = pathList.at(pathList.count() - 2);
            const QString &appName = pathList.at(pathList.count() - 3);

            QJsonDocument document = QJsonDocument::fromJson(outKey.toLocal8Bit());
            const QJsonArray array = document.array();
            QStringList anchors;
            QStringList anchorIdList;
            QStringList anchorInitialList;
            QStringList anchorSpellList;
            QStringList contents;
            bool invalid_entry = false;
            for (const QJsonValue &value : array) {
                if (!value.isArray()) {
                    invalid_entry = true;
                    break;
                }

                const QJsonArray anchor = value.toArray();
                const QString id = anchor.at(0).toString();
                anchorIdList.append(id);
                const QString title_ch = anchor.at(1).toString();
                QString title_us = anchor.at(1).toString();
                anchors.append(title_ch);
                if (lang == "zh_CN") {
                    QString str = Dtk::Core::Chinese2Pinyin(title_ch).remove(QRegExp("[1-9]"));
                    anchorSpellList.append(str);
                    if (id == "h0") {
                        QString anchorInitial;
                        for (int i = 0; i < title_ch.length(); i++) {
                            anchorInitial.append(Dtk::Core::Chinese2Pinyin(title_ch.at(i)).left(1));
                        }
                        anchorInitialList.append(anchorInitial);
                    } else {
                        anchorInitialList.append("");
                    }
                } else if (lang == "en_US") {
                    if (id == "h0") {
                        QStringList listTitle = title_us.split(" ");
                        QString anchorInitial;
                        for (QString str : listTitle) {
                            anchorInitial.append(str.left(1));
                        }
                        anchorInitialList.append(anchorInitial);
                    } else {
                        anchorInitialList.append("");
                    }
                    anchorSpellList.append(title_us.remove(" "));
                }
                const QString content = anchor.at(2).toString();
                contents.append(content);
            }

            if (!invalid_entry) {
                //            qDebug() << "add search entry" << app_name << locale << anchors << endl;
                dbObj->addSearchEntry(appName, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
            }
        }
    }
}

void helperManager::dbusSend(const QStringList &deleteList, const QStringList &addList)
{
    QStringList list;
    list << deleteList;
    list << addList;

    qDebug() << Q_FUNC_INFO << list;
    if (!list.isEmpty()) {
        QDBusMessage msg =
            QDBusMessage::createMethodCall("com.deepin.Manual.FilesUpdate",
                                           "/com/deepin/Manual/FilesUpdate",
                                           "local.ManualFilesUpdateProxy",
                                           "OnFilesUpdate");
        msg << list;
        QDBusMessage response = QDBusConnection::sessionBus().call(msg);

        if (response.type() == QDBusMessage::ReplyMessage) {
            qDebug() << Q_FUNC_INFO << "ReplyMessage";
        }

        if (QDBusMessage::ErrorMessage == response.type()) {
            qDebug() << Q_FUNC_INFO << "ErrorMessage: " << QDBusConnection::sessionBus().lastError().message();
        }
    }

}

void helperManager::onFilelistChange(QStringList deleteList, QStringList addList, QStringList addTime)
{
    handleDb(deleteList, addList, addTime);
}




