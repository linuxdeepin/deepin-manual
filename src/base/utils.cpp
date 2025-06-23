// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "utils.h"
#include "ddlog.h"

#include <DSysInfo>
DCORE_USE_NAMESPACE

#include <QFontDatabase>
#include <QImageReader>

#include "base/consts.h"

QHash<QString, QPixmap> Utils::m_imgCacheHash;
QHash<QString, QString> Utils::m_fontNameCache;
QString Utils::cpuModeName;

const char kLauncherServiceV20[] = "com.deepin.dde.daemon.Launcher";
const char kLauncherIfaceV20[] = "/com/deepin/dde/daemon/Launcher";
const char kLauncherServiceV23[] = "org.deepin.dde.daemon.Launcher1";
const char kLauncherIfaceV23[] = "/org/deepin/dde/daemon/Launcher1";

//标题映射表
const int langCount = 5;
//key zh_Cn en_Us zh_HK zh_TW
QString languageArr[][langCount] = {
    //dde model
    {"controlcenter", "控制中心", "Control Center", "控制中心", "控制中心"},
    {"", "首页介绍", "Homepage Introduction", "首頁介紹", "首頁介紹"},
    {"authentication", "生物认证", "Biometric Authentication", "生物認證", "生物認證"},
    {"passkey", "安全密钥", "Security key", "安全密鑰", "安全金鑰"},
    {"accounts", "帐户设置", "Accounts", "帳戶設定", "帳戶設定"},
    {"cloudsync", "Union ID", "Union ID", "Union ID", "Union ID"},
    {"display", "显示设置", "Display", "螢幕設定", "螢幕設定"},
    {"defapp", "默认程序设置", "Default Applications", "預設程式", "預設程式"},
    {"personalization", "个性化设置", "Personalization Settings", "個性化設定", "個性化設定"},
    {"network", "网络设置", "Network Settings", "網路設定", "網路設定"},
    {"notification", "通知设置", "Notification Settings", "通知設定", "通知設定"},
    {"sound", "声音设置", "Sound Settings", "聲音設定", "聲音設定"},
    {"bluetooth", "蓝牙设置", "Bluetooth Settings", "藍牙設定", "藍牙設定"},
    {"datetime", "时间和格式", "Date and Format", "時間和格式", "時間和格式"},
    {"power", "电源管理", "Power Management", "電源管理", "電源管理"},
    {"mouse", "鼠标和触控板", "Mouse and Touchpad", "滑鼠和觸控板", "滑鼠和觸控板"},
    {"tablet", "数位板", "Drawing Tablet", "數位板", "數位板"},
    {"keyboard", "键盘和语言", "Keyboard and Language", "鍵盤和語言", "鍵盤和語言"},
    {"voice", "辅助功能", "Assistive Tools", "輔助功能", "輔助功能"},
    {"privacy", "隐私和安全", "Privacy and Security", "私隱和安全", "隱私和安全"},
    {"update", "系统更新", "Update Settings", "檢查更新", "檢查更新"},
    {"systeminfo", "系统信息", "System Info", "系統資訊", "系統資訊"},
    {"License activator", "授权管理", "Authorization Management", "授權管理", "授權管理"},
    {"Backup and Restore", "备份还原", "Backup and Restore", "備份還原", "備份還原"},
    {"commoninfo", "通用设置", "General Settings", "一般設定", "一般設定"},
    {"touchscreen", "触控屏设置", "Touch Screen", "觸控屏設定", "觸控屏設定"},
};

struct ReplyStruct {
    QString m_desktop;
    QString m_name;
    QString m_key;
    QString m_iconKey;

    qint64 m_categoryId;
    qint64 m_installedTime;
    QStringList m_appmessage;
};

Q_DECLARE_METATYPE(ReplyStruct)

/**
 * @brief operator <<
 * @param argument
 * @param info
 * @return
 */
QDBusArgument &operator<<(QDBusArgument &argument, const ReplyStruct &info)
{
    argument.beginStructure();
    argument << info.m_desktop << info.m_name << info.m_key << info.m_iconKey;
    if (DSysInfo::majorVersion().toInt() >= 23)
        argument << info.m_categoryId << info.m_installedTime << info.m_appmessage;
    else
        argument << info.m_categoryId << info.m_installedTime;
    argument.endStructure();
    return argument;
}

/**
 * @brief operator >> //结构体数据检查
 * @param argument
 * @param info
 * @return
 */
const QDBusArgument &operator>>(const QDBusArgument &argument, ReplyStruct &info)
{
    argument.beginStructure();
    argument >> info.m_desktop >> info.m_name >> info.m_key >> info.m_iconKey;
    if (DSysInfo::majorVersion().toInt() >= 23)
        argument >> info.m_categoryId >> info.m_installedTime >> info.m_appmessage;
    else
        argument >> info.m_categoryId >> info.m_installedTime;
    argument.endStructure();
    return argument;
}

Utils::Utils(QObject *parent)
    : QObject(parent)
{
    qCDebug(app) << "Utils instance created";
}

Utils::~Utils()
{
    qCDebug(app) << "Utils instance destroyed";
}

/**
 * @brief Utils::renderSVG
 * @param filePath 文件路径
 * @param size 图标大小
 * @return
 * 根据传入的路径，大小，应用信息得到像素图
 */
QPixmap Utils::renderSVG(const QString &filePath, const QSize &size)
{
    qCDebug(app) << "Rendering SVG:" << filePath << "with size:" << size;
    if (m_imgCacheHash.contains(filePath)) {
        qCDebug(app) << "Using cached image for:" << filePath;
        return m_imgCacheHash.value(filePath);
    }

    QImageReader reader;
    QPixmap pixmap;

    reader.setFileName(filePath);
    qCDebug(app) << "SVG file path:" << filePath;

    if (reader.canRead()) {
        qCDebug(app) << "SVG can be read, device pixel ratio:" << qApp->devicePixelRatio();
        const qreal ratio = qApp->devicePixelRatio();
        reader.setScaledSize(size * ratio);
        pixmap = QPixmap::fromImage(reader.read());
        pixmap.setDevicePixelRatio(ratio);
    } else {
        pixmap.load(filePath);
        qCDebug(app) << "SVG cannot be read, loading directly from file:" << filePath;
    }

    m_imgCacheHash.insert(filePath, pixmap);
    qCDebug(app) << "SVG rendered successfully, size:" << pixmap.size();
    return pixmap;
}

/**
 * @brief Utils::translateTitle 返回title映射字段，目前主要用于＂控制中心＂跳转
 * @param titleUS
 * @return
 * @note 除简体中文外,其他暂时都当英文.
 */
QString Utils::translateTitle(const QString &titleUS)
{
    qCDebug(app) << "Translating title:" << titleUS;
    QString strRet = titleUS;
    QString strlocal(QLocale::system().name());
    int nCount = sizeof(languageArr) / sizeof(languageArr[0]);
    for (int i = 0; i < nCount; i++) {
        if (languageArr[i][0] == titleUS) {
            if (0 == strlocal.compare("ug_CN") || 0 == strlocal.compare("bo_CN") || 0 == strlocal.compare("zh_CN")) {
                strRet = languageArr[i][1];
            } else if (0 == strlocal.compare("zh_HK")) {
                strRet = languageArr[i][3];
            } else if (0 == strlocal.compare("zh_TW")) {
                strRet = languageArr[i][4];
            } else {
                strRet = languageArr[i][2];
            }
            qCDebug(app) << "Translated title:" << strRet;
        }
    }
    return strRet;
}

/**
 * @brief Utils::launcherInterface
 * @return 返回系统所有应用列表
 */
QList<AppInfo> Utils::launcherInterface()
{
    qCDebug(app) << "Getting launcher interface";
    QList<AppInfo> applist;

    qRegisterMetaType<ReplyStruct>("ReplyStruct");
    qDBusRegisterMetaType<ReplyStruct>();
    qRegisterMetaType<QList<ReplyStruct>>("a");
    qDBusRegisterMetaType<QList<ReplyStruct>>();

    bool bV23 = DSysInfo::majorVersion().toInt() >= 23;
    QString sLauncherService = bV23 ? kLauncherServiceV23 : kLauncherServiceV20;
    QString sLauncherIface = bV23 ? kLauncherIfaceV23 : kLauncherIfaceV20;
    QDBusInterface iface(sLauncherService, sLauncherIface, sLauncherService, QDBusConnection::sessionBus());
    //root权限下此dbus接口无效...
    if (!iface.isValid()) {
        qCDebug(app) << qPrintable(QDBusConnection::sessionBus().lastError().message());
        qCDebug(app) << QString("majorVersion:%1, servie:%2 iface:%3").arg(DSysInfo::majorVersion()).arg(sLauncherService).arg(sLauncherIface);
        return applist;
//        exit(1);
    }

    QDBusReply<QList<ReplyStruct>> reply = iface.callWithArgumentList(QDBus::CallMode::AutoDetect, "GetAllItemInfos", QVariantList());
    qCDebug(app) << reply.error().message();

    if (reply.isValid()) {
        QList<ReplyStruct> list;
        list = reply.value();

        for (int var = 0; var < list.size(); ++var) {
            AppInfo app;
            app.key = list.at(var).m_key;
            app.name = list.at(var).m_name;
            app.desktop = list.at(var).m_desktop;
            app.category_id = list.at(var).m_categoryId;
            app.installed_time = list.at(var).m_installedTime;
            applist.append(app);
        }
        qCDebug(app) << applist.size() << "applist " << applist;
        return applist;
    } else {
        qCDebug(app) << "GetAllItemInfos fail! " << reply.error().message();
        qCDebug(app) << QString("majorVersion:%1, servie:%2 iface:%3").arg(DSysInfo::majorVersion()).arg(sLauncherService).arg(sLauncherIface);
        return applist;
    }
}

/**
 * @brief Utils::judgeWayLand
 * @return
 * 判断是否为wayland
 */
bool Utils::judgeWayLand()
{
    qCDebug(app) << "Checking for Wayland environment";
    auto env = QProcessEnvironment::systemEnvironment();

    QString XDG_SESSION_TYPE = env.value(QStringLiteral("XDG_SESSION_TYPE"));

    QString WAYLAND_DISPLAY = env.value(QStringLiteral("WAYLAND_DISPLAY"));

    if (XDG_SESSION_TYPE == QLatin1String("wayland") || WAYLAND_DISPLAY.contains(QLatin1String("wayland"), Qt::CaseInsensitive)) {
        qCDebug(app) << "Wayland environment detected";
        return true;
    }

    qCDebug(app) << "XDG_SESSION_TYPE:" << XDG_SESSION_TYPE << "WAYLAND_DISPLAY:" << WAYLAND_DISPLAY;
    return false;
}

QStringList Utils::getMdsourcePath()
{
    qCDebug(app) << "Getting manual source paths";
    QStringList sourcePath;
    QStringList pathlist = getEnvsourcePath();
    qCDebug(app) << "Environment source paths:" << pathlist;

    for (int i = 0; i < pathlist.size(); ++i) {
        QString manualPath = pathlist[i] + "/deepin-manual/manual-assets";
        qCDebug(app) << "Checking manual path:" << manualPath;
        sourcePath.push_back(manualPath);
    }

    qCDebug(app) << "Adding default manual directory:" << DMAN_MANUAL_DIR;
    sourcePath.push_back(DMAN_MANUAL_DIR);

    qCDebug(app) << "Final manual source paths:" << sourcePath;
    return sourcePath;
}

QStringList Utils::getEnvsourcePath()
{
    QString xdgDataDirs = qgetenv("XDG_DATA_DIRS");
    qCDebug(app) << "XDG_DATA_DIRS environment variable:" << xdgDataDirs;

    QStringList pathlist = xdgDataDirs.split(':');
    if (pathlist.size() == 1 && pathlist[0].isEmpty()) {
        qCDebug(app) << "XDG_DATA_DIRS is empty, using default path";
        pathlist[0] = "/usr/share";
    }

    qCDebug(app) << "Final environment source paths:" << pathlist;
    return pathlist;
}

QString Utils::getDesktopFilePath(const QString &desktopname)
{
    qCDebug(app) << "Searching desktop file for:" << desktopname;
    // 遍历XDG_DATA_DIRS中的路径，找寻指定desktop文件
    QStringList pathList = getEnvsourcePath();
    qCDebug(app) << "Searching in paths:" << pathList;

    foreach (auto path, pathList) {
        QString filepath = path + QString("/applications/%1.desktop").arg(desktopname);
        qCDebug(app) << "Checking path:" << filepath;

        QFile file(filepath);
        if (file.exists()) {
            qCDebug(app) << "Found desktop file at:" << filepath;
            return filepath;
        } else {
            qCDebug(app) << "Desktop file not found at:" << filepath;
        }
    }

    qCWarning(app) << "Desktop file not found for:" << desktopname;
    return "";
}

/**
 * @brief Utils::getSystemManualList
 * @return　返回系统中存在帮助手册的应用列表
 */
QStringList Utils::getSystemManualList()
{
    qCDebug(app) << "Getting system manual list";
    QStringList app_list_;
    QStringList strMANUAL_DIR_list = Utils::getMdsourcePath();
    Dtk::Core::DSysInfo::UosEdition type = Utils::uosEditionType();
    foreach (auto strMANUAL_DIR, strMANUAL_DIR_list) {
        const QStringList applicationList = QDir(QString("%1/application/").arg(strMANUAL_DIR)).entryList(QDir::Dirs|QDir::NoDotAndDotDot);
        const QStringList systemList = QDir(QString("%1/system/").arg(strMANUAL_DIR)).entryList(QDir::Dirs|QDir::NoDotAndDotDot);
        QString oldMdPath = strMANUAL_DIR;
        if (Dtk::Core::DSysInfo::UosServer == Dtk::Core::DSysInfo::uosType()) {
            oldMdPath += "/server";
            qCDebug(app) << "server manual path:" << oldMdPath;
        } else if (Dtk::Core::DSysInfo::UosHome == type) {
            oldMdPath += "/personal";
            qCDebug(app) << "personal manual path:" << oldMdPath;
        } else if (Dtk::Core::DSysInfo::UosEducation == type) {
            oldMdPath += "/education";
            qCDebug(app) << "education manual path:" << oldMdPath;
        } else if (Dtk::Core::DSysInfo::UosCommunity == type) {
            oldMdPath += "/community";
            qCDebug(app) << "community manual path:" << oldMdPath;
        } else {
            oldMdPath += "/professional";
            qCDebug(app) << "professional manual path:" << oldMdPath;
        }
        qCDebug(app) << "old manual path:" << oldMdPath;
        const QStringList oldAppList = QDir(oldMdPath).entryList(QDir::Dirs|QDir::NoDotAndDotDot);
        for(auto app:applicationList){
            if (app_list_.indexOf(app) == -1) {
                app_list_.append(app);
            }
        }
        if (systemList.contains("dde") || oldAppList.contains("dde")) {
            if (app_list_.indexOf("dde") == -1) {
                app_list_.append("dde");
            }
            qCDebug(app) << "app_list_:" << app_list_ << ", count:" << app_list_.size();
        }
        // 只有在专业版下面才显示快速帮助
        if (systemList.contains(kLearnBasicOperations) || oldAppList.contains(kLearnBasicOperations)) {
            qCDebug(app) << "systemList contains:" << kLearnBasicOperations;
            if (Dtk::Core::DSysInfo::UosProfessional == type && app_list_.indexOf(kLearnBasicOperations) == -1)
                app_list_.append(kLearnBasicOperations);
        }
        if (systemList.contains(kCommonApplicationLibraries) || oldAppList.contains(kCommonApplicationLibraries)) {
            qCDebug(app) << "systemList contains:" << kCommonApplicationLibraries;
            if (Dtk::Core::DSysInfo::UosProfessional == type && app_list_.indexOf(kCommonApplicationLibraries) == -1 )
                app_list_.append(kCommonApplicationLibraries);
        }
        qCDebug(app) << "exist app list: " << app_list_ << ", count:" << app_list_.size();
    }
    qCDebug(app) << "exist app list: " << app_list_ << ", count:" << app_list_.size();
    return app_list_;
}

/**
 * @brief Utils::getSystemManualDir
 * @return
 * @note　获取系统版本信息
 */
QStringList Utils::getSystemManualDir()
{
    qCDebug(app) << "system manual dir:" << Utils::getMdsourcePath();
    return Utils::getMdsourcePath();
}

/**
 * @brief Utils::sortAppList
 * @param map
 * @return 返回排序后的ａｐｐｌｉｓｔ
 * @note appList　排序,如果安装时间相同时按字母前后排序
 */
QList<AppInfo> Utils::sortAppList(QMultiMap<qlonglong, AppInfo> map)
{
    qCDebug(app) << "Sorting application list";
    QList<AppInfo> listEnd;
    QList<AppInfo> listtmp;
    qlonglong longlongtmp = 0;
    QMultiMap<qlonglong, AppInfo>::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        //只在第一次循环时插入listtemp
        if (listtmp.isEmpty()) {
            listtmp.append(it.value());
            longlongtmp = it.key();
            ++it;
            qCDebug(app) << "First app:" << it.value().name;
            continue;
        }

        //如果这本次key与longlongtemp相等说明，当前应用的安装时间与上一次循环中的应用安装时间相同，把appInfo插入listtemp等待排序
        //如果不相等，listtemp不为null，则对littemp按"应用包名"进行排序；
        //并把排序结果添加到listEnd;
        //清空listtmp, 修改longlongtmp记录当前key;
        if (it.key() == longlongtmp) {
            listtmp.append(it.value());
            qCDebug(app) << "Same time app:" << it.value().name;
        } else if (!listtmp.isEmpty() && it.key() != longlongtmp) {
            AppInfo m;
            for (int i = 0; i < listtmp.size(); ++i) {
                for (int j = 0; j < listtmp.size() - 1; ++j) {
                    if (listtmp.at(j).key > listtmp.at(j + 1).key) {
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
            qCDebug(app) << "Sort app:" << it.value().name;
        }
        ++it;
        qCDebug(app) << "Next app:" << it.value().name;
    }
    //最后判断listtmp是否为空，处理循环结束时，最后几次longlongtmp都是相等的情况
    if (!listtmp.isEmpty()) {
        qCDebug(app) << "Sort last app:" << listtmp.at(0).name;
        QList<AppInfo> temp;
        {
            AppInfo m;
            for (int i = 0; i < listtmp.size(); ++i) {
                for (int j = 0; j < listtmp.size() - 1; ++j) {
                    if (listtmp.at(j).key > listtmp.at(j + 1).key) {
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
    qCDebug(app) << "Sort end app size:" << listEnd.size();
    return listEnd;
}

/**
 * @brief Utils::hasSelperSupport
 * @return
 * @note 判断是否需要显示服务与支持功能
 */
bool Utils::hasSelperSupport()
{
    qCDebug(app) << "hasSelperSupport";
    Dtk::Core::DSysInfo::UosEdition type = uosEditionType();
    //专业版判断是否有服务与支持
    if (Dtk::Core::DSysInfo::UosProfessional == type || Dtk::Core::DSysInfo::UosMilitary == type || Dtk::Core::DSysInfo::UosMilitaryS == type) {
        qCDebug(app) << "hasSelperSupport true";
        const QStringList list = getSystemManualList();
        if (list.contains("uos-service-support")) {
            qCDebug(app) << "uos-service-support found in manual list";
            return true;
        }
    }
    qCDebug(app) << "hasSelperSupport false";
    return false;
}

bool checkOsBuildValid(const QString& str)
{
    // Format like ABCDE.xyz.abc
    static QRegularExpression regex("^[1-9A-Z][1-9A-Z][0-9A-Z][1-9A-Z][1-9A-Z](\\.\\w+)*$");

    qCDebug(app) << "checkOsBuildValid:" << str;

    return regex.match(str).hasMatch();
}

Dtk::Core::DSysInfo::UosEdition Utils::uosEditionType()
{
    // Add static cache, query only once    
    static Dtk::Core::DSysInfo::UosEdition cachedEdition = Dtk::Core::DSysInfo::UosEdition::UosEditionUnknown;
    static bool hasQueried = false;
    qCDebug(app) << "uosEditionType";
    
    // If already queried, return cached result
    if (hasQueried) {
        qCDebug(app) << "uosEditionType cached";
        return cachedEdition;
    }
    
    QDBusInterface interface("org.deepin.dde.SystemInfo1",
                            "/org/deepin/dde/SystemInfo1",
                            "org.deepin.dde.SystemInfo1",
                            QDBusConnection::sessionBus());

    if(!interface.isValid()) {
        qCDebug(app) << "SystemInfo DBus interface invalid";
        hasQueried = true;
        return cachedEdition;
    }

    QVariant osBuild = interface.property("OsBuild");
    if(!osBuild.isValid()) {
        qCDebug(app) << "Failed to get OsBuild property";
        hasQueried = true;
        return cachedEdition;
    }

    QString buildType = osBuild.toString();
    if(buildType.isEmpty()) {
        qCDebug(app) << "OsBuild property is empty";
        hasQueried = true;
        return cachedEdition;
    }

    QStringList parts = buildType.split(".");
    qCDebug(app) << "OsBuild parts:" << parts;
    if(parts.size() != 3) {
        qCDebug(app) << "Invalid OsBuild format, expected 3 parts";
        hasQueried = true;
        return cachedEdition;
    }

    // Save result to cache
    cachedEdition = parseOsBuildType(parts[0]);
    hasQueried = true;
    qCDebug(app) << "Parsed OsBuild type:" << cachedEdition;
    return cachedEdition;
}

Dtk::Core::DSysInfo::UosEdition Utils::parseOsBuildType(const QString & osBuild)
{
    qCDebug(app) << "parseOsBuildType:" << osBuild;
    if (!checkOsBuildValid(osBuild)) {
        qCDebug(app) << "Invalid OsBuild format:" << osBuild;
        return Dtk::Core::DSysInfo::UosEditionUnknown;
    }

    int uosType = osBuild[1].digitValue();
    int uosEditType = osBuild[3].digitValue();
    // copy from dtk-core/src/dsysinfo.cpp
    if(uosType == 1) {
        qCDebug(app) << "Uos type 1";
        switch (uosEditType) {
        case 1:
            qCDebug(app) << "UosProfessional";
            return Dtk::Core::DSysInfo::UosEdition::UosProfessional;
        case 2:
        case 7:
            //The new version of the family version (7) and the old version of the personal version (2) The same as the home does not modify the old logic (7) to ensure the adaptation of the old version
            qCDebug(app) << "UosHome";
            return Dtk::Core::DSysInfo::UosEdition::UosHome;
        case 3:
            qCDebug(app) << "UosCommunity";
            return Dtk::Core::DSysInfo::UosEdition::UosCommunity;
        case 4:
        case 9:
            qCDebug(app) << "UosMilitary";
            return Dtk::Core::DSysInfo::UosEdition::UosMilitary;
        case 5:
            qCDebug(app) << "UosDeviceEdition";
            return Dtk::Core::DSysInfo::UosEdition::UosDeviceEdition;
        case 6:
            qCDebug(app) << "UosEducation";
            return Dtk::Core::DSysInfo::UosEdition::UosEducation;
        default:
            qCDebug(app) << "UosEditionUnknown";
            break;
        }

    }else if(uosType == 2) {
        qCDebug(app) << "Uos type 2";
        switch (uosEditType) {
        case 1:
            qCDebug(app) << "UosEnterprise";
            return Dtk::Core::DSysInfo::UosEdition::UosEnterprise;
        case 2:
            qCDebug(app) << "UosProfessionalC";
            return Dtk::Core::DSysInfo::UosEdition::UosEnterpriseC;
        case 3:
            qCDebug(app) << "UosEuler";
            return Dtk::Core::DSysInfo::UosEdition::UosEuler;
        case 4:
        case 9:
            qCDebug(app) << "UosMilitaryS";
            return Dtk::Core::DSysInfo::UosEdition::UosMilitaryS;
        case 5:
            qCDebug(app) << "UosDeviceEdition";
            return Dtk::Core::DSysInfo::UosEdition::UosDeviceEdition;
        default:
            qCDebug(app) << "UosEditionUnknown";
            break;
        }
    }
    else if (uosType == 3){
        qCDebug(app) << "Uos type 3";
        return Dtk::Core::DSysInfo::UosEnterprise;
    }

    qCDebug(app) << "final retun UosEditionUnknown";
    return Dtk::Core::DSysInfo::UosEditionUnknown;
}

bool Utils::hasAppStore()
{
    qCDebug(app) << "hasAppStore";
    const QStringList list = getSystemManualList();
    if (list.contains("deepin-app-store")) {
        qCDebug(app) << "deepin-app-store found in manual list";
        return true;
    } else {
        qCDebug(app) << "deepin-app-store not found in manual list";
        return false;
    }
}

//p表示桌面专业版，h表示个人版，d表示社区版
//s表示默认服务器版，e表示服务器企业版，eu表示服务器欧拉版，i表示服务器行业版，klu表示KelvinU项目版本，pgv表示PanguV项目版本。
QStringList Utils::systemToOmit(Dtk::Core::DSysInfo::UosEdition type)
{
    qCDebug(app) << "systemToOmit";
    QStringList retList;
    switch (type) {
    qCDebug(app) << "type" << type;
    //专业版
    case  Dtk::Core::DSysInfo::UosProfessional:
    case  Dtk::Core::DSysInfo::UosMilitary:
        qCDebug(app) << "omit p";
        retList.append("p");
        break;
    //个人版
    case  Dtk::Core::DSysInfo::UosHome:
        qCDebug(app) << "omit h";
        retList.append("h");
        break;
    //社区版
    case  Dtk::Core::DSysInfo::UosCommunity:
        qCDebug(app) << "omit d";
        retList.append("d");
        break;
    //服务器企业版
    case  Dtk::Core::DSysInfo::UosEnterprise:
    case  Dtk::Core::DSysInfo::UosMilitaryS:
        qCDebug(app) << "omit e,s";
        retList.append("e");
        retList.append("s");
        break;
    //服务器行业版
    case  Dtk::Core::DSysInfo::UosEnterpriseC:
        qCDebug(app) << "omit i,s";
        retList.append("i");
        retList.append("s");
        break;
    //服务器欧拉版
    case  Dtk::Core::DSysInfo::UosEuler:
        qCDebug(app) << "omit eu,s";
        retList.append("eu");
        retList.append("s");
        break;
    //教育版
    case Dtk::Core::DSysInfo::UosEducation:
        qCDebug(app) << "omit edu";
        retList.append("edu");
        break;
    default:
        qCDebug(app) << "omit default";
        break;
    }
    qCDebug(app) << "omit list:" << retList;
    return retList;
}

bool Utils::activeWindow(quintptr winId)
{
    qCDebug(app) << "activeWindow" << winId;
    bool bsuccess = true;
    // new interface use applicationName as id
    QString dockService = "com.deepin.dde.daemon.Dock";
    QString dockPath = "/com/deepin/dde/daemon/Dock";
    QString dockInterface = "com.deepin.dde.daemon.Dock";
    if (DSysInfo::majorVersion().toInt() >= 23) {
        dockService = "org.deepin.dde.daemon.Dock1";
        dockPath = "/org/deepin/dde/daemon/Dock1";
        dockInterface = "org.deepin.dde.daemon.Dock1";
    }
    QDBusInterface manual(dockService, dockPath, dockInterface);
    QDBusReply<void> reply = manual.call("ActivateWindow", winId);
    if (!reply.isValid()) {
        qCDebug(app) << "call" << dockService << "failed" << reply.error();
        bsuccess = false;
    }
    qCDebug(app) << "activeWindow result:" << bsuccess;
    return bsuccess;
}

QString Utils::regexp_label(const QString &strtext, const QString &strpatter)
{
    qCDebug(app) << "regexp_label called with text:" << strtext << "pattern:" << strpatter;
    QString strsource = strtext;
    QString result;
    QRegularExpression re(strpatter, QRegularExpression::MultilineOption | QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = re.match(strsource);
    if (match.isValid() && match.hasMatch()) {
        qCDebug(app) << "match! regexp_label captured:" << match.captured(0);
        for (int i = 0; i <= match.lastCapturedIndex(); i++) {
            result = match.captured(i);
            qCDebug(app) << "break! regexp_label captured:" << result;
            break;
        }
    }
    qCDebug(app) << "regexp_label result:" << result;
    return result;
}

QString Utils::mkMutiDir(const QString &path)
{
    qCDebug(app) << "mkMutiDir called with path:" << path;
    QDir dir(path);
    if (path.isEmpty() || dir.exists()) {
        qCDebug(app) << "mkMutiDir result:" << path;
        return path;
    }
    QString parentDir = mkMutiDir(path.mid(0, path.lastIndexOf('/')));
    QString dirname = path.mid(path.lastIndexOf('/') + 1);
    QDir parentPath(parentDir);
    if (!dirname.isEmpty()) {
        bool ret = parentPath.mkpath(dirname);
        qCDebug(app) << "mkpath result:" << ret << dirname;
    }
    qCDebug(app) << "mkMutiDir result:" << parentDir + "/" + dirname;
    return parentDir + "/" + dirname;
}

/**
 * @brief Utils::judgeLongson
 * @node 判断龙芯
 * @return
 */
bool Utils::judgeLoongson()
{
    qCDebug(app) << "Checking for Loongson CPU";

    if (cpuModeName.isEmpty()) {
        QProcess process;
        //获取CPU型号
        qCDebug(app) << "Starting process to read CPU info";
        process.start("cat /proc/cpuinfo");

        if (process.waitForFinished()) {
            QString result = process.readAllStandardOutput();
            qCDebug(app) << "CPU info read successfully";

            if (result.contains("Loongson")) {
                qCInfo(app) << "cpu mode name is loongson";
                cpuModeName = "Loongson";
            } else {
                qCInfo(app) << "cpu mode name is not loongson";
                cpuModeName = "other";
            }
        }
    }

    bool isLoongson = cpuModeName.contains("Loongson");
    qCDebug(app) << "CPU check result - is Loongson:" << isLoongson;

    return isLoongson;
}

ExApplicationHelper *ExApplicationHelper::instance()
{
    return qobject_cast<ExApplicationHelper *>(DGuiApplicationHelper::instance());
}

#define CAST_INT static_cast<int>

QColor light_qpalette[QPalette::NColorRoles] {
    QColor(0x00, 0x00, 0x00),                         // WindowText
    QColor(0xe5, 0xe5, 0xe5),                         // Button
    QColor(0xe6, 0xe6, 0xe6),                         // Light
    QColor(0xe5, 0xe5, 0xe5),                         // Midlight
    QColor(0xe3, 0xe3, 0xe3),                         // Dark
    QColor(0xe4, 0xe4, 0xe4),                         // Mid
    QColor(0x41, 0x4d, 0x68),                         // Text
    Qt::black,                                   // BrightText
    QColor(0x41, 0x4d, 0x68),                         // ButtonText
    Qt::white,                                   // Base
    QColor(0xf8, 0xf8, 0xf8),                         // Window
    QColor(0, 0, 0, CAST_INT(0.05 * 255)),       // Shadow
    QColor(0x00, 0x81, 0xff),                           // Highlight
    Qt::white,                                   // HighlightedText
    QColor(0x00, 0x82, 0xfa),                         // Link
    QColor(0xad, 0x45, 0x79),                         // LinkVisited
    QColor(0, 0, 0, CAST_INT(0.03 * 255)),       // AlternateBase
    Qt::white,                                   // NoRole
    QColor(255, 255, 255, CAST_INT(0.8 * 255)),  // ToolTipBase
    Qt::black                                    // ToolTipText
};

QColor dark_qpalette[QPalette::NColorRoles] {
    QColor(0xff, 0xff, 0xff),                      // WindowText
    QColor(0x44, 0x44, 0x44),                      // Button
    QColor(0x48, 0x48, 0x48),                      // Light
    QColor(0x47, 0x47, 0x47),                      // Midlight
    QColor(0x41, 0x41, 0x41),                      // Dark
    QColor(0x43, 0x43, 0x43),                      // Mid
    QColor(0xc0, 0xc6, 0xd4),                      // Text
    Qt::white,                                // BrightText
    QColor(0xc0, 0xc6, 0xd4),                      // ButtonText
    QColor(0x28, 0x28, 0x28),                      // Base
    QColor(0x25, 0x25, 0x25),                      // Window
    QColor(0, 0, 0, CAST_INT(0.05 * 255)),    // Shadow
    QColor(0x00, 0x81, 0xff),                      // Highlight
    QColor(0xF1, 0xF6, 0xFF),                      // HighlightedText
    QColor(0x00, 0x82, 0xfa),                      // Link
    QColor(0xad, 0x45, 0x79),                      // LinkVisited
    QColor(0, 0, 0, CAST_INT(0.05 * 255)),    // AlternateBase
    Qt::black,                                // NoRole
    QColor(45, 45, 45, CAST_INT(0.8 * 255)),  // ToolTipBase
    QColor(0xc0, 0xc6, 0xd4)                       // ToolTipText
};

QColor light_dpalette[DPalette::NColorTypes] {
    QColor(), // NoType
    QColor(255, 255, 255, CAST_INT(0.2 * 255)), // ItemBackground
    //    QColor("#001A2E"),                           // TextTitle
    //    QColor("#8AA1B4"),                           // TextTips
    //    QColor("#FF5736"),                           // TextWarning
    //    QColor("#0082FA"),                           // TextLively
    //    QColor("#25b7ff"),                           // LightLively
    QColor(0x00, 0x1A, 0x2E), // TextTitle
    QColor(0x8A, 0xA1, 0xB4), // TextTips
    QColor(0xFF, 0x57, 0x36), // TextWarning
    QColor(0x00, 0x82, 0xFA), // TextLively
    QColor(0x25, 0xb7, 0xff), // LightLively

    QColor(235, 235, 235, CAST_INT(0 * 255)), // DarkLively
    QColor(235, 235, 235, CAST_INT(0 * 255)) // FrameBorder
};

QColor dark_dpalette[DPalette::NColorTypes] {
    QColor(), // NoType
    QColor(25, 25, 25, CAST_INT(0 * 255)), // ItemBackground
    //    QColor("#C0C6D4"),                       // TextTitle
    //    QColor("#6D7C88"),                       // TextTips
    //    QColor("#FF5736"),                       // TextWarning
    //    QColor("#0082FA"),                       // TextLively
    //    QColor("#0056c1"),                       // LightLively
    QColor(0xC0, 0xC6, 0xD4), // TextTitle
    QColor(0x6D, 0x7C, 0x88), // TextTips
    QColor(0xFF, 0x57, 0x36), // TextWarning
    QColor(0x00, 0x82, 0xFA), // TextLively
    QColor(0x00, 0x56, 0xc1), // LightLively
    QColor(25, 25, 25, CAST_INT(0 * 255)), // DarkLively
    QColor(25, 25, 25, CAST_INT(0.5 * 255)) // FrameBorder
};

DPalette ExApplicationHelper::standardPalette(DGuiApplicationHelper::ColorType type) const
{
    qCDebug(app) << "standardPalette called with type:" << type;
    DPalette pa;
    const QColor *qcolor_list, *dcolor_list;

    if (type == DarkType) {
        qcolor_list = dark_qpalette;
        dcolor_list = dark_dpalette;
        qCDebug(app) << "DarkType";
    } else {
        qcolor_list = light_qpalette;
        dcolor_list = light_dpalette;
        qCDebug(app) << "LightType";
    }

    for (int i = 0; i < DPalette::NColorRoles; ++i) {
        QPalette::ColorRole role = static_cast<QPalette::ColorRole>(i);

        QColor color = qcolor_list[i];
        pa.setColor(DPalette::Active, role, color);
        generatePaletteColor(pa, role, type);
        qCDebug(app) << "role:" << role << "color:" << color;
    }

    for (int i = 0; i < DPalette::NColorTypes; ++i) {
        DPalette::ColorType role = static_cast<DPalette::ColorType>(i);

        QColor color = dcolor_list[i];
        pa.setColor(DPalette::Active, role, color);
        generatePaletteColor(pa, role, type);
        qCDebug(app) << "role:" << role << "color:" << color;
    }

    qCDebug(app) << "standardPalette result:" << pa;
    return pa;
}

DPalette ExApplicationHelper::palette(const QWidget *widget, const QPalette &base) const
{
    qCDebug(app) << "palette called with widget:" << widget << "base:" << base;
    Q_UNUSED(base)

    DPalette palette;

    // 存在自定义palette时应该根据其自定义的palette获取对应色调的DPalette
    const QPalette &wp = widget->palette();

    palette = standardPalette(toColorType(wp));

    // 关注控件palette改变的事件
    const_cast<QWidget *>(widget)->installEventFilter(const_cast<ExApplicationHelper *>(this));

    qCDebug(app) << "palette result:" << palette;
    return palette;
}

void ExApplicationHelper::setPalette(QWidget *widget, const DPalette &palette)
{
    qCDebug(app) << "setPalette called with widget:" << widget << "palette:" << palette;
    // 记录此控件被设置过palette
    widget->setProperty("_d_set_palette", true);
    widget->setPalette(palette);
}

ExApplicationHelper::ExApplicationHelper()
{
    qCDebug(app) << "ExApplicationHelper constructor called";
}

ExApplicationHelper::~ExApplicationHelper()
{
    qCDebug(app) << "ExApplicationHelper destructor called";
}

bool ExApplicationHelper::eventFilter(QObject *watched, QEvent *event)
{
    qCDebug(app) << "ExApplicationHelper eventFilter called with watched";
    return DGuiApplicationHelper::eventFilter(watched, event);
}
