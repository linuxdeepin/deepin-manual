// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "helpermanager.h"
#include "base/command.h"
#include "base/utils.h"
#include "base/consts.h"
#include "base/ddlog.h"
#include "controller/filewatcher.h"
#include "controller/search_db.h"
#include "view/jscontext.h"

#include <QDBusMessage>
#include <QtDBus>
#include <QDBusConnection>

helperManager::helperManager(QObject *parent)
    : QObject(parent)
    , watcherObj(new fileWatcher(this))
    , dbObj(new SearchDb(this))
    , timerObj(new QTimer(this))
    , jsObj(new JsContext(this))
{
    timerObj->setSingleShot(true);
    timerObj->setInterval(1000);
    initWeb();
    initDbConfig();
    initConnect();
}

/**
 * @brief helperManager::initWeb 初始化web配置
 */
void helperManager::initWeb()
{
    qCDebug(app) << "initWeb";

    m_webView = new QWebEngineView;
    m_webView->setFixedSize(400, 200);
    connect(m_webView->page(), &QWebEnginePage::loadFinished, this, &helperManager::webLoadFinish);
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
    QStringList  assetsPathList = Utils::getSystemManualDir();
    foreach (auto assetsPath, assetsPathList) {
        for (const QString &type : QDir(assetsPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
            if (type == "system" || type == "application") {
                QString typePath = assetsPath + "/" + type;
                //监控资源文件夹
                for (QString &module : QDir(typePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                    //./manual-assets/application(system)/appName
                    QString modulePath = typePath + "/" + module;
                    QStringList listAppNameT = QDir(modulePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs);

                    if (listAppNameT.count() != 1) {
                        qCritical() << modulePath  << "：there are more folders..:" << listAppNameT.count();
                        continue;
                    }
                    //./manual-assets/application(system)/appName/appNameT
                    QString appPath = modulePath + "/" + listAppNameT.at(0);
                    for (QString &lang : QDir(appPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                        if (0 != lang.compare("common")) {
                            //./manual-assets/application(system)/appName/appNameT/lang
                            QString langPath = appPath + "/" + lang;
                            for (QString &mdFile : QDir(langPath).entryList(QDir::Files)) {
                                if (mdFile.endsWith("md")) {
                                    //./manual-assets/application(system)/appName/appNameT/lang/md
                                    QString strMd = langPath + "/" + mdFile;
                                    QFileInfo fileInfo(strMd);
                                    if (fileInfo.exists()) {
                                        QString modifyTime = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss.zzz");
                                        mapNow.insert(strMd, modifyTime);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

//旧文案结构兼容  /usr/share/deepin-manual/manual-assets/[professional | server]/appName/lang/index.md
#if 1
        for (const QString &system : QDir(assetsPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
            if (systemType.contains(system)) {
                QString typePath = assetsPath + "/" + system;
                for (QString &module : QDir(typePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                    QString modulePath = typePath + "/" + module;
                    for (QString &lang : QDir(modulePath).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
                        // 除了common目录其它其它都是各语言对应文档目录
                        if (0 != lang.compare("common")) {
                            QString strMd = modulePath + "/" + lang + "/index.md";
                            QFileInfo fileInfo(strMd);
                            if (fileInfo.exists()) {
                                QString modifyTime = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss.zzz");
                                mapNow.insert(strMd, modifyTime);
                            }
                        }
                    }
                }
            }
        }
#endif
    }

    QFileInfo fileInfo(kVideoConfigPath);
    if (fileInfo.exists()) {
        QString modifyTime = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss.zzz");
        mapNow.insert(kVideoConfigPath, modifyTime);
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
    connect(timerObj, &QTimer::timeout, this, &helperManager::onTimerOut);
}

/**
 * @brief helperManager::handleDb
 * @param deleteList   比较得出删除的文件
 * @param addList      比较得出新增的文件（文件增加 & 文件被修改）
 * @param addTime      新增文件更新时间
 * @note  mdPath: ./manual-assets/system(application)/appName[应用名称]/appNameT[应用项目名称]/lang/xxx.md
 */
void helperManager::handleDb(const QStringList &deleteList, const QStringList &addList, const QStringList &addTime)
{
    qCDebug(app) << deleteList.count() << " " << addList.count();
    if (!deleteList.isEmpty()) {
        dbObj->deleteFilesTimeEntry(deleteList);
        QStringList appList;
        QStringList langList;
        for (const QString &path : deleteList) {
            if (path.contains("video-guide")) {
                appList << "video-guide" << "video-guide";
                langList << "en_US" << "zh_CN";
            } else {
                QStringList list = path.split("/");
                if (list.count() >= 4) {
                    langList.append(list.at(list.count() - 2));
                    appList.append(list.at(list.count() - 4));
                }
            }
        }

        dbObj->deleteSearchInfo(appList, langList);
    }

    QStringList tmpAddList = addList;
    int videoIndex = tmpAddList.indexOf(kVideoConfigPath);
    if (videoIndex >= 0) { //视频配置文件单独拿出来处理
//        tmpAddList.removeAt(videoIndex);
        QFile file(kVideoConfigPath);
        if (!file.open(QIODevice::ReadOnly)) {
            qCDebug(app) << "Failed to open video config file.";
        } else {
            QByteArray jsonData = file.readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
            if (jsonDoc.isNull()) {
                qCDebug(app) << "Failed to parse JSON of video config file.";
            } else {
                QJsonObject jsonObj = jsonDoc.object();

                QMap<QString, QStringList> anchors;
                QStringList anchorIdList;
                QMap<QString, QStringList> anchorInitialList;
                QMap<QString, QStringList> anchorSpellList;
                QStringList contents;

                for (QString key : jsonObj.keys()) {
                    QJsonValue jsValue = jsonObj.value(key);
                    if (jsValue.isArray() && key == "videos") {
                        QJsonArray jsArray = jsValue.toArray();
                        QJsonArray resultArray;
                        for (int i = 0; i < jsArray.count(); i++) {
                            if (jsArray[i].isObject()) {
                                QJsonObject obj = jsArray[i].toObject();
                                if (obj["name[en_US]"].isString() && obj["name[zh_CN]"].isString()) {
                                    anchors["en_US"].append(obj["name[en_US]"].toString());
                                    anchors["zh_CN"].append(obj["name[zh_CN]"].toString());
                                    anchors["zh_HK"].append(obj["name[zh_HK]"].toString());
                                    anchors["zh_TW"].append(obj["name[zh_TW]"].toString());

                                    anchorInitialList["en_US"].append("");
                                    anchorInitialList["zh_CN"].append("");
                                    anchorInitialList["zh_HK"].append("");
                                    anchorInitialList["zh_TW"].append("");

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
                                    QRegExp regExp("[1-9]");
#else
                                    QRegularExpression regExp("[1-9]");
#endif

                                    anchorSpellList["en_US"].append(obj["name[en_US]"].toString().remove(" "));
                                    anchorSpellList["zh_CN"].append(Dtk::Core::Chinese2Pinyin(obj["name[zh_CN]"].toString()).remove(regExp));
                                    anchorSpellList["zh_HK"].append("");
                                    anchorSpellList["zh_TW"].append("");

                                    anchorIdList.append(QString("h%0").arg(i));
                                    contents.append(obj["url"].toString());
                                }
                            }
                        }
                    }
                    for(QString lang_key : anchors.keys()) {
                        if (anchorInitialList.keys().contains(lang_key) && anchorSpellList.keys().contains(lang_key))
                            dbObj->addSearchEntry("video-guide", lang_key, anchors[lang_key], anchorInitialList[lang_key], anchorSpellList[lang_key], anchorIdList, contents);
                    }
                }
            }
        }
    }

    QStringList list = handlePriority(tmpAddList);

    if (!list.isEmpty() && !addTime.isEmpty()) {
        dbObj->insertFilesTimeEntry(tmpAddList, addTime);
        //通过JS层函数来完成md转html, 然后解析html内所有文本内容
        if (jsObj && m_webView) {
            QString strChange = list.join(",");
            qCDebug(app) << strChange;

            m_webView->page()->runJavaScript(QString("parseMdList('%1')").arg(strChange));
        }
    }

    deleteTList = deleteList;
    addTList = list;
    timerObj->start();
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

    qCDebug(app) << list;
    if (!list.isEmpty()) {
        QDBusMessage msg =
            QDBusMessage::createMethodCall("com.deepin.Manual.FilesUpdate",
                                           "/com/deepin/Manual/FilesUpdate",
                                           "local.ManualFilesUpdateProxy",
                                           "OnFilesUpdate");
        msg << list;
        QDBusMessage response = QDBusConnection::sessionBus().call(msg);

        if (response.type() == QDBusMessage::ReplyMessage) {
            qCDebug(app) << "ReplyMessage";
        } else if (response.type() == QDBusMessage::ErrorMessage) {
            qCDebug(app) << "ErrorMessage: " << QDBusConnection::sessionBus().lastError().message();
        }
    }
}

/**
 * @brief helperManager::handlePriority
 * @param list  修改的md文件列表
 * @return  返回列表，同类型优先级最高的一个。
 * @note 先获取当前系统类型优先字段排序， 然后再判断对应的优先文件是否存，返回优先列表
 */
QStringList helperManager::handlePriority(const QStringList &list)
{
    qCDebug(app) << list;
    QStringList omitType = Utils::systemToOmit(Utils::uosEditionType());
    qCDebug(app) << omitType;
    QStringList moduleList;
    QStringList retList;
    //key: md路径  value:md文件是否为最高优先级别
    QMap<QString, bool> map;

    for (int i = 0; i < list.count(); i++) {
        QString mdPath = list.at(i);
        QStringList splitList = mdPath.split("/");

        //新文案结构 /usr/share/deepin-manual/manual-assets/[application | system]/appName/appNameT/land/*_appNameT.md
        if (splitList.contains("application") || splitList.contains("system")) {
            QString moduleLang = splitList.at(splitList.count() - 4) + splitList.at(splitList.count() - 2);
            QString mdFile = splitList.at(splitList.count() - 1);
            QStringList listTemp = mdFile.split("_");
            // 首位如果包含在系统类型里， 则代表其为特定系统类型文件*_appName.md，否则为默认文件appName.md
            if (!systemList.contains(listTemp.at(0))) {
                //如果之前没有对应的模块语言的md文件， 则直接添加
                if (!moduleList.contains(moduleLang)) {
                    moduleList.append(moduleLang);
                    retList.append(mdPath);
                }
                //如果之前存在对应的模块语言的md文件，则判断对应文件是否为最高优先级， 不是最高优先级，则添加。
                else if (!map.value(moduleLang)) {
                    int nIndex = moduleList.indexOf(moduleLang);
                    if (nIndex != -1) {
                        moduleList.removeAt(nIndex);
                        retList.removeAt(nIndex);
                    }
                    moduleList.append(moduleLang);
                    map.insert(moduleLang, false);
                    retList.append(mdPath);
                }
            } else if (systemList.contains(listTemp.at(0)) && omitType.contains(listTemp.at(0))) {
                int nIndex = moduleList.indexOf(moduleLang);
                if (nIndex != -1) {
                    moduleList.removeAt(nIndex);
                    retList.removeAt(nIndex);
                }
                moduleList.append(moduleLang);
                map.insert(moduleLang, true);
                retList.append(mdPath);
            }
        }

//旧文案结构兼容  /usr/share/deepin-manual/manual-assets/[professional | server]/appName/lang/index.md
#if 1
        if (splitList.count() == 9 && systemType.contains(splitList.at(5))) {
            QString moduleLang = splitList.at(splitList.count() - 3) + splitList.at(splitList.count() - 2);
            if (!moduleList.contains(moduleLang)) {
                moduleList.append(moduleLang);
                map.insert(moduleLang, false);
                retList.append(mdPath);
            }
        }
#endif
    }
    return  retList;
}

void helperManager::onFilelistChange(QStringList deleteList, QStringList addList, QStringList addTime)
{
    handleDb(deleteList, addList, addTime);
}

void helperManager::webLoadFinish(bool ok)
{
    qCDebug(app) << "dmanHelper load web======>" << ok;
    getModuleInfo();
}

/**
 * @brief helperManager::onRecvParseMsg  md文件内容处理， 插入数据库
 * @param msg  解析md返回的内容
 * @param path 解析md文件的路径  ./manual-assets/application(system)/appName/appNameT/lang/md
 */
void helperManager::onRecvParseMsg(const QString &msg, const QString &path)
{
    QStringList pathList = path.split("/");
    QString lang = pathList.at(pathList.count() - 2);
    QString appName = pathList.at(pathList.count() - 4);

//旧文案结构兼容  /usr/share/deepin-manual/manual-assets/[professional | server]/appName/lang/index.md
#if 1
    if (systemType.contains(pathList.at(pathList.count() - 4))) {
        lang = pathList.at(pathList.count() - 2);
        appName = pathList.at(pathList.count() - 3);
    }
#endif

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
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
            QRegExp regExp("[1-9]");
#else
            QRegularExpression regExp("[1-9]");
#endif
            QString str = Dtk::Core::Chinese2Pinyin(title_ch).remove(regExp);
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
        } else {
            anchorInitialList.append("");
            anchorSpellList.append("");
        }
        const QString content = anchor.at(2).toString();
        contents.append(content);
    }

    if (!invalid_entry) {
        dbObj->addSearchEntry(appName, lang, anchors, anchorInitialList, anchorSpellList, anchorIdList, contents, path);
    }
    //获取内容解析返回 重置定时器
    timerObj->start();
}

/**
 * @brief helperManager::onTimerOut 定时器 通知前端刷新
 */
void helperManager::onTimerOut()
{
    if (!deleteTList.isEmpty() || !addTList.isEmpty()) {
        dbusSend(deleteTList, addTList);
    }
}
