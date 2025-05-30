// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "view/manual_proxy.h"
#include "controller/config_manager.h"
#include "base/consts.h"
#include "base/utils.h"
#include "base/ddlog.h"

#include <DDesktopEntry>

#include <QIcon>
#include <QDesktopServices>
#include <QDir>
#include <QSettings>
#include <QBuffer>

ManualProxy::ManualProxy(QObject *parent)
    : QObject(parent)
    , strIconTheme("")
{
    AppInfo::registerMetaType();
}

ManualProxy::~ManualProxy()
{
}

/**
 * @brief ManualProxy::getSystemManualDir
 * @return 帮助手册的版本信息 专业版 、服务器版
 * 获取帮助手册版本信息
 */
QStringList ManualProxy::getSystemManualDir()
{
    return Utils::getSystemManualDir();;
}

/**
 * @brief ManualProxy::getSystemManualList
 * @return 应用列表
 * 获取存在帮助手册的应用列表
 */
QStringList ManualProxy::getSystemManualList()
{
    QStringList list = Utils::getSystemManualList();
    saveAppList(list);
    qCDebug(app) << "System manual list:" << list;
    return list;
}

/**
 * @brief ManualProxy::setApplicationState
 * @param appName
 * 应用图标点击后修改配置文件
 */
void ManualProxy::setApplicationState(const QString &appName)
{
    QString strApp;
    //部分传的是完整路径,部分直接是模块名称
    if (appName.contains("%2F")) {
        //以"%2F"分割字符,取倒数第三位为具体模块名
        QStringList strlist = appName.split("%2F");
        strApp = strlist.at(strlist.count() - 3);
    } else {
        strApp = appName;
    }
    qCInfo(app) << "Setting application state:" << strApp;

    QSettings *setting = ConfigManager::getInstance()->getSettings();
    setting->beginGroup(QString(kConfigAppList));
    if (setting->contains(strApp)) {
        setting->setValue(strApp, false);
        qCDebug(app) << setting->applicationName() << setting->fileName() << ": " << appName << " state=false";
    } else {
        setting->setValue(strApp, false);
        qCDebug(app) << setting->fileName() << ": " << strApp << " not find";
    }
    setting->endGroup();
}

/**
 * @brief ManualProxy::getUsedAppList
 * @return
 * 获取已经使用过的应用列表
 */
QStringList ManualProxy::getUsedAppList()
{
    QSettings *setting = ConfigManager::getInstance()->getSettings();
    setting->beginGroup(QString(kConfigAppList));
    QStringList list = setting->allKeys();
    QStringList appList;
    for (int i = 0; i < list.size(); ++i) {
        if (!setting->value(list.at(i)).toBool()) {
            appList.append(list.at(i));
        } else {
            continue;
        }
    }
    setting->endGroup();
    qCDebug(app) << "Used application list:" << appList;
    return appList;
}

/**
 * @brief ManualProxy::hasSelperSupport
 * @return
 * @note 显示服务与支持功能条件：　专业版并且已经安装 "uos-service-support"应用
 */
bool ManualProxy::hasSelperSupport()
{
    bool b = Utils::hasSelperSupport();
    return b;
}

bool ManualProxy::hasAppStore()
{
    bool b = Utils::hasAppStore();
    return b;
}

/**
 * @brief ManualProxy::finishChannel
 * 完成channel对象与Qt对象绑定后调用WebWindow中的onChannelFinish方法
 */
void ManualProxy::finishChannel()
{
    emit channelInit();
}

/**
 * @brief ManualProxy::supportClick
 * 服务与支持
 */
void ManualProxy::supportClick()
{
    emit supportBeClick();
}

void ManualProxy::appStoreClick()
{
    emit appStoreBeClick();
}

bool ManualProxy::bIsLongSon()
{
    return Utils::judgeLoongson();
}

void ManualProxy::showUpdateLabel()
{
    emit updateLabel();
}

//根据app名称找到对应md文件
QString ManualProxy::appToPath(const QString &appName)
{
    QStringList omitType = Utils::systemToOmit(Utils::uosEditionType());
    QStringList mdList;
    QStringList  assetsPathList = Utils::getSystemManualDir();
    foreach (auto assetPath, assetsPathList) {
        QString appPath;
        if (appName == "dde" || appName == kLearnBasicOperations || appName == kCommonApplicationLibraries || appName == "video-guide") {
            appPath = assetPath + "/system/" + appName;
        } else {
            appPath = assetPath + "/application/" + appName;
        }

        if (QDir(appPath).exists()) {
            QStringList list = QDir(appPath).entryList(QDir::NoDotAndDotDot | QDir::Dirs);
            QString appNameT;
            if (list.count() == 1) {
                appNameT = list.at(0);
            } else if (list.count() > 1) {
                qCWarning(app) << assetPath << "dir greater than one:" << list;
                appNameT = list.at(1);
            } else {
                appNameT = "error";
                qCWarning(app) << " no dir";
            }
            appPath.append("/").append(appNameT).append("/");
            appPath = getAppLocalDir(appPath);

            if (omitType.length() > 1) {
                mdList.append(appPath + "/" + QString("%1_%2.md").arg(omitType.at(0)).arg(appNameT));
                mdList.append(appPath + "/" + QString("%1_%2.md").arg(omitType.at(1)).arg(appNameT));
            } else {
                if (omitType.size() > 0) {
                    mdList.append(appPath + "/" + QString("%1_%2.md").arg(omitType.at(0)).arg(appNameT));
                }
            }
            //根据文档命名规则不带前缀文档为所有文档的兜底文档
            mdList.append(appPath + "/" + QString("%1.md").arg(appNameT));
        }

        QString oldMdPath = assetPath;

#if (DTK_VERSION > DTK_VERSION_CHECK(5, 4, 12, 0))
        if (Dtk::Core::DSysInfo::UosServer == Dtk::Core::DSysInfo::uosType()) {
            oldMdPath += "/server";
        } else if (Dtk::Core::DSysInfo::UosHome == Utils::uosEditionType()) {
            oldMdPath += "/personal";
        } else if (Dtk::Core::DSysInfo::UosEducation == Utils::uosEditionType()) {
            oldMdPath += "/education";
        } else if (Dtk::Core::DSysInfo::UosCommunity == Utils::uosEditionType()) {
            oldMdPath += "/community";
        } else {
            oldMdPath += "/professional";
        }
#else
        Dtk::Core::DSysInfo::DeepinType nType = Dtk::Core::DSysInfo::deepinType();
        if (Dtk::Core::DSysInfo::DeepinServer == nType) {
            oldMdPath += "/server";
        } else if (Dtk::Core::DSysInfo::DeepinPersonal == nType) {
            oldMdPath += "/personal";
        } else {
            if (Dtk::Core::DSysInfo::isCommunityEdition()) {
                oldMdPath += "/community";
            } else {
                oldMdPath += "/professional";
            }
        }

#endif
        oldMdPath.append("/").append(appName).append("/");
        oldMdPath = getAppLocalDir(oldMdPath);
        mdList.append(oldMdPath.append("/index.md"));
    }

    QString ret;
    for (auto md : mdList) {
        if (QFile(md).exists()) {
            ret = md;
        }
    }
    if (ret.isEmpty()) {
        qCWarning(app) << " no exist file:" << appName;
        // TODO(wurongjie) 在之前的代码中返回了error,暂不知作用
        return "error";
    }

    return ret;
}


//根据dde子项未匹配到md key则翻译
QString ManualProxy::translateTitle(const QString &titleUS)
{
    return Utils::translateTitle(titleUS);
}

//根据应用desktop文件解析图标名称并获取图标路径
QString ManualProxy::getAppIconPath(const QString &desktopname)
{
    //首次获取默认图标主题，如果获取失败默认bloom
    if (strIconTheme.isEmpty()) {
        QFile file("/usr/share/glib-2.0/schemas/com.deepin.dde.appearance.gschema.xml");
        if (file.exists() && file.open(QIODevice::ReadOnly)) {
            QString strContent(file.readAll());
            strContent = Utils::regexp_label(strContent, "(icon-theme\">\n)(.*)?(['</default>])");
            strIconTheme = Utils::regexp_label(strContent, "(?<=<default>')(.*)?(?='</default>)");
        }
        if (strIconTheme.isEmpty()) {
            strIconTheme = "bloom";
        }
    }

    QString filepath = Utils::getDesktopFilePath(desktopname);
    QFile file(filepath);
    QString strIcon = desktopname;
    if (file.exists()) {
        Dtk::Core::DDesktopEntry entry(filepath);
        strIcon = entry.stringValue("Icon");
    } else {
        qCWarning(app) << QString("filepath:%1 not exist.. desktopname:%2").arg(filepath).arg(desktopname);
    }

    qCDebug(app) << "Icon name:" << strIcon;

    if (QIcon::hasThemeIcon(strIcon)) {
        QIcon icon = QIcon::fromTheme(strIcon);
        if (!icon.isNull()) {
            // 强制目标尺寸为 96x96， 与前端的展示尺寸一致，避免转换成base64的时候超大无法正常显示
            QSize targetSize(96, 96);
            QPixmap pixmap = icon.pixmap(targetSize);
            if (!pixmap.isNull()) {
                QByteArray byteArray;
                QBuffer buffer(&byteArray);
                buffer.open(QIODevice::WriteOnly);
                pixmap.save(&buffer, "PNG");

                // 转换为base64
                QString base64 = byteArray.toBase64();
                QString dataUrl = QString("data:image/png;base64,%1").arg(base64);
                return dataUrl;
            } else {
                qCWarning(app) << "Failed to get pixmap for" << strIcon;
            }
        } else {
            qCWarning(app) << "Icon is null for:" << strIcon;
        }
    } else {
        qCWarning(app) << "No theme icon found for:" << strIcon;
    }

    qCWarning(app) << "Failed to get icon for:" << strIcon;
    return "";
}

QString ManualProxy::getLocalAppName(const QString &desktopname)
{
    QString strdisplayname = desktopname;
    if (0 == desktopname.compare(kLearnBasicOperations, Qt::CaseInsensitive)) {
        strdisplayname = tr("Learn Basic Operations");
    } else if (0 == desktopname.compare(kCommonApplicationLibraries, Qt::CaseInsensitive)) {
        strdisplayname = tr("Common Application Libraries");
    } else if (0 == desktopname.compare("dde", Qt::CaseInsensitive)) {
        strdisplayname = tr("Desktop Environment");
    } else {
        QStringList pathList = Utils::getEnvsourcePath();
        foreach (auto path, pathList) {
            QString filepath = path + QString("/applications/%1.desktop").arg(desktopname);
            QFile file(filepath);
            if (file.exists()) {
                Dtk::Core::DDesktopEntry entry(filepath);
                strdisplayname = entry.genericName();
                strdisplayname = strdisplayname.isEmpty() ? entry.ddeDisplayName() : strdisplayname;
                return strdisplayname;
            }
        }
    }
    return strdisplayname;
}

QVariant ManualProxy::getVideoGuideInfo()
{
    // 社区版不出现视频指南，返回空进行屏蔽
    if (Utils::uosEditionType() == Dtk::Core::DSysInfo::UosEdition::UosCommunity)
        return QVariantList();

    QFile file(kVideoConfigPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(app) << "Failed to open file";
        return QVariantList();
    }

    QString locale = QLocale().name();
    //藏语维语使用简体中文
    if (locale == "ug_CN" || locale == "bo_CN") {
        locale = "zh_CN";
    } else if (locale == "en_US" || locale == "en_GB") {
        locale = "en_US";
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull()) {
        qCWarning(app) << "Failed to parse JSON";
        return QVariantList();
    }

//    locale = "en_US"; //test
    QJsonObject jsonObj = jsonDoc.object();

    for (QString key : jsonObj.keys()) {
        QJsonValue jsValue = jsonObj.value(key);

        if (jsValue.isString() && key == "url") {
            videoUrl = jsValue.toString();
        } else if (jsValue.isArray() && key == "videos") {
            QJsonArray jsArray = jsValue.toArray();
            QJsonArray resultArray;

            for (int i = 0; i < jsArray.count(); i++) {
                if (jsArray[i].isObject()) {
                    QJsonObject obj = jsArray[i].toObject();
                    QJsonObject tmpObj {
                        {"cover", QString(DMAN_MANUAL_DIR"/system/video-guide/videos/" + obj["cover"].toString())},
                        {"url", obj["url"]}
                    };

                    if (locale == "zh_CN") {
                        tmpObj.insert("name", obj["name[zh_CN]"]);
                    } else if(locale == "zh_HK") {
                        tmpObj.insert("name", obj["name[zh_HK]"]);
                    } else if(locale == "zh_TW") {
                        tmpObj.insert("name", obj["name[zh_TW]"]);
                    } else {
                        tmpObj.insert("name", obj["name[en_US]"]);
                    }

                    resultArray.append(tmpObj);
                }
            }

            return resultArray.toVariantList();
        }
    }

    return QVariantList();
}

void ManualProxy::openVideo(QString url)
{
    if(url.isEmpty()) {
        url = videoUrl;
    }
    QDesktopServices::openUrl(url);
}

/**
 * @brief ManualProxy::saveAppList
 * @param list
 * 保存应用列表至配置文件并设置状态
 */
void ManualProxy::saveAppList(const QStringList &list)
{
    Q_UNUSED(list)
    QSettings *setting = ConfigManager::getInstance()->getSettings();
    //获取配置文件中应用列表
    setting->beginGroup(QString(kConfigAppList));
    for (int i = 0; i < list.size(); ++i) {
        if (setting->contains(list.at(i))) {
            continue;
        } else {
            //应用不存在则保存
            setting->setValue(list.at(i), true);
        }
    }
    QStringList l = setting->allKeys();
    setting->endGroup();
    qCDebug(app) << "app config  allKeys count : " << l.size();
}

QString ManualProxy::getAppLocalDir(const QString &appPath)
{
    //appPath like /usr/share/deepin-manual/manual-assets/application/deepin-boot-maker/boot-maker/
    QString appdir(appPath);
    QString strlocal(QLocale::system().name());
    QString AppLocalDir = QString(appPath).append(strlocal);
    QDir dir(AppLocalDir);
    //如果不存在该种语言的文档路径，藏语、维语使用简体中文，其它使用英文

    if (!dir.exists()) {
        //藏语维语使用简体中文
        if (0 == strlocal.compare("ug_CN") || 0 == strlocal.compare("bo_CN")
                || 0 == strlocal.compare("zh_HK") || 0 == strlocal.compare("zh_TW")) {
            AppLocalDir = QString(appPath).append("zh_CN");
        } else {
            AppLocalDir = QString(appPath).append("en_US");
        }
    }

    return AppLocalDir;
}

