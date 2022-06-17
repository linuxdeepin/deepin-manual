/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "view/manual_proxy.h"
#include "controller/config_manager.h"
#include "base/consts.h"
#include "base/utils.h"

#include <DDesktopEntry>

#include <QtGui/private/qiconloader_p.h>

ManualProxy::ManualProxy(QObject *parent)
    : QObject(parent)
    , strIconTheme("")
    , piconload(new QIconLoader)
{
    piconload->ensureInitialized();
    AppInfo::registerMetaType();
}

ManualProxy::~ManualProxy()
{
    if (nullptr != piconload)
        delete piconload;
}

/**
 * @brief ManualProxy::getSystemManualDir
 * @return 帮助手册的版本信息 专业版 、服务器版
 * 获取帮助手册版本信息
 */
QString ManualProxy::getSystemManualDir()
{
    QString str = Utils::getSystemManualDir();
    return str;
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
    qDebug() << "======================>" << list;
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
    qDebug() << "open app---->" << strApp;

    QSettings *setting = ConfigManager::getInstance()->getSettings();
    setting->beginGroup(kConfigAppList);
    if (setting->contains(strApp)) {
        setting->setValue(strApp, false);
        qDebug() << setting->applicationName() << setting->fileName() << ": " << appName << " state=false";
    } else {
        setting->setValue(strApp, false);
        qDebug() << setting->fileName() << ": " << strApp << " not find";
    }
    setting->endGroup();
    setting->sync();
}

/**
 * @brief ManualProxy::getUsedAppList
 * @return
 * 获取已经使用过的应用列表
 */
QStringList ManualProxy::getUsedAppList()
{
    QSettings *setting = ConfigManager::getInstance()->getSettings();
    setting->beginGroup(kConfigAppList);
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
    qDebug() << "The application of already used： " << appList;
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
    qDebug() << __FUNCTION__ << "========>" << appName;
    QStringList omitType = Utils::systemToOmit(Dtk::Core::DSysInfo::uosEditionType());
    const QString assetPath = Utils::getSystemManualDir();
    QStringList mdList;
    QString appPath;
    if (appName == "dde") {
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
            qWarning() << Q_FUNC_INFO << assetPath << "dir greater than one:" << list;
            appNameT = list.at(1);
        } else {
            appNameT = "error";
            qWarning() << Q_FUNC_INFO << " no dir";
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
    } else if (Dtk::Core::DSysInfo::UosHome == Dtk::Core::DSysInfo::uosEditionType()) {
        oldMdPath += "/personal";
    } else if (Dtk::Core::DSysInfo::UosEducation == Dtk::Core::DSysInfo::uosEditionType()) {
        oldMdPath += "/education";
    } else if (Dtk::Core::DSysInfo::UosCommunity == Dtk::Core::DSysInfo::uosEditionType()) {
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

    qInfo() << mdList;
    //初始化赋值，如果为空字符，web层路径请求依旧能onload成功...
    QString ret = "error";
    if (QFile(mdList[0]).exists()) {
        ret = mdList[0];
    } else if (mdList.length() > 1 && QFile(mdList[1]).exists()) {
        ret = mdList[1];
    } else if (mdList.length() > 2 && QFile(mdList[2]).exists()) {
        ret = mdList[2];
    } else if (mdList.length() > 3 && QFile(mdList[3]).exists()) {
        ret = mdList[3];
    } else {
        qWarning() << Q_FUNC_INFO << " no exist file:" << appName;
    }
    qInfo() << "========>" << ret;
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

    QString filepath = QString("/usr/share/applications/%1.desktop").arg(desktopname);
    QFile file(filepath);
    QString strIcon = desktopname;
    if (file.exists()) {
        Dtk::Core::DDesktopEntry entry(filepath);
        strIcon = entry.stringValue("Icon");
        qDebug() << strIcon;
    }

    QString strIconPath;
    if (QIcon::hasThemeIcon(strIcon) && nullptr != piconload) {
        //强制设置主题为默认主题保证获取的图标来自与默认图标主题
        piconload->setThemeName(strIconTheme);
        QThemeIconInfo info = piconload->loadIcon(strIcon);

        QString str96, str64, str48, str36;
        for (int i = 0; i < info.entries.size(); i++) {
            //修改变量名 cppcheck错误 20210419
            QString filepathtmp = info.entries.at(i)->filename;
            if (filepathtmp.contains("96")) {
                str96 = filepathtmp;
                break;
            } else if (filepathtmp.contains("64")) {
                str64 = filepathtmp;
            } else if (filepathtmp.contains("48")) {
                str48 = filepathtmp;
            } else if (filepathtmp.contains("36")) {
                str36 = filepathtmp;
            } else {
                strIconPath = filepathtmp;
            }
        }
        if (!str96.isEmpty()) {
            strIconPath = str96;
        } else if (!str64.isEmpty()) {
            strIconPath = str64;
        } else if (!str48.isEmpty()) {
            strIconPath = str48;
        } else if (!str36.isEmpty()) {
            strIconPath = str36;
        }
    }

    return strIconPath;
}

QString ManualProxy::getLocalAppName(const QString &desktopname)
{
    QString strdisplayname = desktopname;
    if (0 == desktopname.compare("dde", Qt::CaseInsensitive)) {
        strdisplayname = tr("Desktop Environment");
    } else {
        QString filepath = QString("/usr/share/applications/%1.desktop").arg(desktopname);
        QFile file(filepath);
        if (file.exists()) {
            Dtk::Core::DDesktopEntry entry(filepath);
            strdisplayname = entry.genericName();
            strdisplayname = strdisplayname.isEmpty() ? entry.ddeDisplayName() : strdisplayname;
        }
    }
    return strdisplayname;
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
    setting->beginGroup(kConfigAppList);
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
    setting->sync();
    qDebug() << "app config  allKeys count : " << l.size();
}

QString ManualProxy::getAppLocalDir(const QString &appPath)
{
    //appPath like /usr/share/deepin-manual/manual-assets/application/deepin-boot-maker/boot-maker/
    QString appdir(appPath);
    QString strlocal(QLocale::system().name());
    QString AppLocalDir = QString(appPath).append(strlocal);
    QDir dir(AppLocalDir);
    //如果不存在该种语言的文档路径，藏语、维语使用简体中文，其它使用英文
    qInfo() << __FUNCTION__ << dir.absolutePath() << strlocal;
    if (!dir.exists()) {
        //藏语维语使用简体中文
        if (0 == strlocal.compare("ug_CN") || 0 == strlocal.compare("bo_CN")
            || 0 == strlocal.compare("zh_HK") || 0 == strlocal.compare("zh_TW")) {
            AppLocalDir = QString(appPath).append("zh_CN");
        } else {
            AppLocalDir = QString(appPath).append("en_US");
        }
    }
    qInfo() << __FUNCTION__ << "AppLocalDir:" << AppLocalDir;
    return AppLocalDir;
}

