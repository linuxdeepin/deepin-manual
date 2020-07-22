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

namespace dman {

namespace {

/**
 * Read manual id from app desktop file.
 * @param desktop_file Absolute path to app desktop file.
 * @return Returns manual id if exists or else returns empty string.
 */
QString GetDeepinManualId(const QString &desktop_file)
{
    QSettings settings(desktop_file, QSettings::IniFormat);
    settings.beginGroup("Desktop Entry");
    const QVariant value = settings.value("X-Deepin-ManualID");
    if (value.isValid()) {
        return value.toString();
    }
    return "";
}

}  // namespace

ManualProxy::ManualProxy(QObject *parent)
    : QObject(parent)
    , launcher_interface_(new LauncherInterface(kLauncherService, kLauncherIface,
                                                QDBusConnection::sessionBus(), this))
{
    AppInfo::registerMetaType();
}

//Installation time phase at the same time, sorted by appName
QList<AppInfo> ManualProxy::sortAppList(QMultiMap<qlonglong, AppInfo> map)
{
    QMapIterator<qlonglong, AppInfo> it(map);
    QList<AppInfo> listEnd;
    QList<AppInfo> listtmp;
    qlonglong longlongtmp = 0;
    while (it.hasNext()) {
        it.next();
        if (it.value().name == map.first().name) {
            listtmp.append(it.value());
            longlongtmp = it.key();
            continue;
        }
        if (it.key() == longlongtmp) {
            listtmp.append(it.value());
        } else if (listtmp.size() != 0 && it.key() != longlongtmp) {
            AppInfo m;
            for (int i = 0; i < listtmp.size(); ++i) {
                for (int j = 0; j < listtmp.size() - 1; ++j) {
                    if (listtmp.at(j).name > listtmp.at(j + 1).name) {
                        m = listtmp.at(j);
                        listtmp[j] = listtmp[j + 1];
                        listtmp[j + 1] = m;
                    }
                }
            }
            listEnd.append(listtmp);
            listtmp.clear();
            longlongtmp = it.key();
            listtmp.append(it.value());
        }
    }
    if (!listtmp.isEmpty()) {
        QList<AppInfo> temp;
        {
            AppInfo m;
            for (int i = 0; i < listtmp.size(); ++i) {
                for (int j = 0; j < listtmp.size() - 1; ++j) {
                    if (listtmp.at(j).name > listtmp.at(j + 1).name) {
                        m = listtmp.at(j);
                        listtmp[j] = listtmp[j + 1];
                        listtmp[j + 1] = m;
                    }
                }
            }
            temp.append(listtmp);
        }
        listEnd.append(temp);
    }
    return listEnd;
}

ManualProxy::~ManualProxy() {}

QString ManualProxy::getSystemManualDir()
{
    QString strMANUAL_DIR = DMAN_MANUAL_DIR;
    int nType = Dtk::Core::DSysInfo::deepinType();
    if (Dtk::Core::DSysInfo::DeepinServer == (Dtk::Core::DSysInfo::DeepinType)nType) {
        strMANUAL_DIR += "/server";
    } else if (Dtk::Core::DSysInfo::DeepinPersonal == (Dtk::Core::DSysInfo::DeepinType)nType) {
        strMANUAL_DIR += "/personal";
    } else {
        if (Dtk::Core::DSysInfo::isCommunityEdition()) {
            strMANUAL_DIR += "/community";
        } else {
            strMANUAL_DIR += "/professional";
        }
    }
    return strMANUAL_DIR;
}

QStringList ManualProxy::getSystemManualList()
{
    const QHash<QString, QString> kAppNameMap = {
        {"org.deepin.flatdeb.deepin-calendar", "dde-calendar"},
        {"org.deepin.flatdeb.deepin-music", "deepin-music"},
        {"org.deepin.flatdeb.deepin-screenshot", "deepin-screenshot"},
        {"org.deepin.flatdeb.deepin-voice-recorder", "deepin-voice-recorder"},
        {"deepin-cloud-print-configurator", "deepin-cloud-print"},
        {"org.deepin.flatdeb.deepin-image-viewer", "deepin-image-viewer"},
        {"deepin-cloud-scan-configurator", "deepin-cloud-scan"},
        {"org.deepin.flatdeb.deepin-movie", "deepin-movie"},
        {"org.deepin.flatdeb.deepin-screen-recorder", "deepin-screen-recorder"},
        {"org.deepin.flatdeb.deepin-calculator", "deepin-calculator"},
        {"com.deepin.editor", "deepin-editor"},
        {"org.deepin.scaner", "scaner"},
    };


    if (app_list_.isEmpty()) {
        const QStringList dir_entry = QDir(this->getSystemManualDir()).entryList();
        const AppInfoList list = launcher_interface_->GetAllItemInfos();
        qDebug() << __func__ << "get all item-->" << list;

        QMultiMap<qlonglong, AppInfo> appMap;
        for (int var = 0; var < list.size(); ++var) {
            appMap.insert(list.at(var).installed_time, list.at(var));
        }
        //Installation time phase at the same time, sorted by name
        QList<AppInfo> listApp = sortAppList(appMap);

        for (int i = 0; i < listApp.size(); ++i) {
            const QString app_name = kAppNameMap.value(listApp.at(i).key, listApp.at(i).key);
            if ((dir_entry.indexOf(app_name) != -1) && app_list_.indexOf(app_name) == -1) {
                app_list_.append(app_name);
            }
            const QString deepin_app_id = GetDeepinManualId(listApp.at(i).desktop);
            if (deepin_app_id == app_name && app_list_.indexOf(app_name) == -1) {
                app_list_.append(app_name);
            }
        }
        // Add "dde" by hand, as it has no desktop file.
        if (dir_entry.contains("dde")) {
            app_list_.append("dde");
        }
        // Remove youdao-dict if current locale is not Simplified Chinese.
        if (!QLocale().name().startsWith("zh")) {
            app_list_.removeAll("youdao-dict");
        }
    }
    qDebug() << "exist app list====:" << app_list_ << ", count:" << app_list_.size();
    return app_list_;
}

void ManualProxy::openExternalLink(const QString &url)
{
    qDebug() << "ManualProxy::openExternalLink：" << url;

    QDesktopServices::openUrl(url);
}

/*** 应用图标点击后修改配置文件 2020-06-18 16:30:17 wangml ***/
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
    setting->beginGroup(CONFIG_APPLIST);
    if (setting->contains(appName)) {
        setting->setValue(appName, false);
        qDebug() << setting->applicationName() << setting->fileName() << ": " << appName << " state=false";
    } else {
        qDebug() << setting->fileName() << ": " << strApp << " not find";
    }
    setting->endGroup();
}

/*** 获取已经使用过的应用列表 2020-06-18 16:29:15 wangml ***/
QStringList ManualProxy::getUsedAppList()
{
    QSettings *setting = ConfigManager::getInstance()->getSettings();
    setting->beginGroup(CONFIG_APPLIST);
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

void ManualProxy::finishChannel()
{
    emit channelInit();
}

/*** 保存应用列表至配置文件并设置状态 2020-06-18 16:32:52 wangml ***/
void ManualProxy::saveAppList(const QStringList &list)
{
    Q_UNUSED(list)
    QSettings *setting = ConfigManager::getInstance()->getSettings();
    setting->beginGroup(CONFIG_APPLIST);
    for (int i = 0; i < list.size(); ++i) {
        if (setting->contains(list.at(i))) {
            continue;
        } else {
            setting->setValue(list.at(i), true);
        }
    }
    QStringList l = setting->allKeys();
    setting->endGroup();
    qDebug() << "app config  allKeys count : " << l.size();

}

}  // namespace dman

//bool ManualPro
