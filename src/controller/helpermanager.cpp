#include "helpermanager.h"
#include "base/command.h"
#include "base/utils.h"
#include "base/consts.h"
#include "controller/filewatcher.h"
#include "controller/search_db.h"
#include "view/jscontext.h"

#include <QDBusMessage>
#include <QtDBus>
#include <QDBusConnection>

helperManager::helperManager(QObject *parent)
    : QObject(parent)
    , watcherObj(new fileWatcher)
    , dbObj(new SearchDb)
{
    initWeb();
    initDbConfig();
    initConnect();
}

/**
 * @brief helperManager::initWeb 初始化web配置
 */
void helperManager::initWeb()
{
    qDebug() << Q_FUNC_INFO;
    m_webView = new QWebEngineView;
    m_webView->setFixedSize(400, 200);
    connect(m_webView->page(), &QWebEnginePage::loadFinished, this, &helperManager::webLoadFinish);
    jsObj = new JsContext(this);
    m_webChannel = new QWebChannel(this);
    m_webChannel->registerObject("context", jsObj);
    m_webView->page()->setWebChannel(m_webChannel);
    const QFileInfo info(kSearchIndexPage);
    m_webView->load(QUrl::fromLocalFile(info.absoluteFilePath()));
}

/**
 * @brief helperManager::initDbConfig 初始化数据库配置
 */
void helperManager::initDbConfig()
{
    dbObj->initDb();
    dbObj->initSearchTable();
    dbObj->initTimeTable();
}

/**
 * @brief helperManager::getModuleInfo 白名单对比
 */
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
    connect(jsObj, &JsContext::parseMsg, this, &helperManager::onRecvParseMsg);
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

        //通过JS层函数来完成md转html, 然后解析html内所有文本内容
        if (jsObj) {
            QString strChange;
            for (int i = 0; i < addList.count(); i++) {
                strChange += addList[i];
                if (i < addList.count() - 1) {
                    strChange += ",";
                }
            }
            qDebug() << Q_FUNC_INFO << strChange;
            m_webView->page()->runJavaScript(QString("parseMdList('%1')").arg(strChange));
        }
    }

    dbusSend(deleteList, addList);
}

/**
 * @brief helperManager::dbusSend dbus通知刷新
 * @param deleteList 删除的文件列表
 * @param addList    更新的文件列表
 */
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
        } else if (response.type() == QDBusMessage::ErrorMessage) {
            qDebug() << Q_FUNC_INFO << "ErrorMessage: " << QDBusConnection::sessionBus().lastError().message();
        }
    }
}

void helperManager::onFilelistChange(QStringList deleteList, QStringList addList, QStringList addTime)
{
    handleDb(deleteList, addList, addTime);
}

void helperManager::webLoadFinish(bool ok)
{
    qDebug() << "dmanHelper load web======>" << ok;
    getModuleInfo();
}

/**
 * @brief helperManager::onRecvParseMsg  md文件内容处理， 插入数据库
 * @param msg  解析md返回的内容
 * @param path 解析md文件的路径
 */
void helperManager::onRecvParseMsg(const QString &msg, const QString &path)
{
    QStringList pathList = path.split("/");
    if (pathList.count() < 3) return;
    const QString &lang = pathList.at(pathList.count() - 2);
    const QString &appName = pathList.at(pathList.count() - 3);

    QJsonDocument document = QJsonDocument::fromJson(msg.toLocal8Bit());
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
        dbObj->addSearchEntry(appName, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents);
    }
}




