// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "utils.h"

#include <DSysInfo>
DCORE_USE_NAMESPACE

#include <QFontDatabase>
#include <QImageReader>
#ifdef DTKCORE_CLASS_DConfigFile
#include <DConfig>
#endif

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
    {"accounts", "账户设置", "Accounts", "帳戶設定", "帳戶設定"},
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
    if (DSysInfo::majorVersion() == "23")
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
    if (DSysInfo::majorVersion() == "23")
        argument >> info.m_categoryId >> info.m_installedTime >> info.m_appmessage;
    else
        argument >> info.m_categoryId >> info.m_installedTime;
    argument.endStructure();
    return argument;
}

Utils::Utils(QObject *parent)
    : QObject(parent)
{
}

Utils::~Utils()
{
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
    if (m_imgCacheHash.contains(filePath)) {
        return m_imgCacheHash.value(filePath);
    }

    QImageReader reader;
    QPixmap pixmap;

    reader.setFileName(filePath);

    if (reader.canRead()) {
        const qreal ratio = qApp->devicePixelRatio();
        reader.setScaledSize(size * ratio);
        pixmap = QPixmap::fromImage(reader.read());
        pixmap.setDevicePixelRatio(ratio);
    } else {
        pixmap.load(filePath);
    }

    m_imgCacheHash.insert(filePath, pixmap);

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
    QList<AppInfo> applist;

    qRegisterMetaType<ReplyStruct>("ReplyStruct");
    qDBusRegisterMetaType<ReplyStruct>();
    qRegisterMetaType<QList<ReplyStruct>>("a");
    qDBusRegisterMetaType<QList<ReplyStruct>>();

    bool bV23 = DSysInfo::majorVersion() == "23";
    QString sLauncherService = bV23 ? kLauncherServiceV23 : kLauncherServiceV20;
    QString sLauncherIface = bV23 ? kLauncherIfaceV23 : kLauncherIfaceV20;
    QDBusInterface iface(sLauncherService, sLauncherIface, sLauncherService, QDBusConnection::sessionBus());
    //root权限下此dbus接口无效...
    if (!iface.isValid()) {
        qDebug() << qPrintable(QDBusConnection::sessionBus().lastError().message());
        qDebug() << QString("majorVersion:%1, servie:%2 iface:%3").arg(DSysInfo::majorVersion()).arg(sLauncherService).arg(sLauncherIface);
        return applist;
//        exit(1);
    }

    QDBusReply<QList<ReplyStruct>> reply = iface.callWithArgumentList(QDBus::CallMode::AutoDetect, "GetAllItemInfos", QVariantList());
    qDebug() << reply.error().message();

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
            //qDebug() << "dbusMsg ---- : " << var << list.at(var).m_name;
        }
//        qDebug() << applist.size() << "applist " <<  applist;
        return applist;
    } else {
        qDebug() << "GetAllItemInfos fail! " << reply.error().message();
        qDebug() << QString("majorVersion:%1, servie:%2 iface:%3").arg(DSysInfo::majorVersion()).arg(sLauncherService).arg(sLauncherIface);
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
    auto env = QProcessEnvironment::systemEnvironment();

    QString XDG_SESSION_TYPE = env.value(QStringLiteral("XDG_SESSION_TYPE"));

    QString WAYLAND_DISPLAY = env.value(QStringLiteral("WAYLAND_DISPLAY"));

    if (XDG_SESSION_TYPE == QLatin1String("wayland") || WAYLAND_DISPLAY.contains(QLatin1String("wayland"), Qt::CaseInsensitive)) {
        return true;
    }

    return false;
}

QStringList Utils::getMdsourcePath()
{
    QStringList sourcePath;
    QStringList pathlist = getEnvsourcePath();
    for (int i = 0; i < pathlist.size(); ++i) {
        sourcePath.push_back(pathlist[i] + "/deepin-manual/manual-assets");
    }
    sourcePath.push_back(DMAN_MANUAL_DIR);
    qDebug() << " all MD source path : " << sourcePath.last();
    return sourcePath;
}

QStringList Utils::getEnvsourcePath()
{
    QStringList pathlist = QString(qgetenv("XDG_DATA_DIRS")).split(':');
    if (pathlist.size() == 1 && pathlist[0].isEmpty())
        pathlist[0] = "/usr/share";
    qDebug() << " all source path : " << pathlist;
    return pathlist;
}

QString Utils::getDesktopFilePath(const QString &desktopname)
{
    // 遍历XDG_DATA_DIRS中的路径，找寻指定desktop文件
    QStringList pathList = getEnvsourcePath();
    foreach (auto path, pathList) {
        QString filepath = path + QString("/applications/%1.desktop").arg(desktopname);
        QFile file(filepath);
        if (file.exists()) {
            return filepath;
        }
    }

    return "";
}

/**
 * @brief Utils::getSystemManualList
 * @return　返回系统中存在帮助手册的应用列表
 */
QStringList Utils::getSystemManualList()
{
    QStringList app_list_;
    QStringList strMANUAL_DIR_list = Utils::getMdsourcePath();
    foreach (auto strMANUAL_DIR, strMANUAL_DIR_list) {
        const QStringList applicationList = QDir(QString("%1/application/").arg(strMANUAL_DIR)).entryList(QDir::Dirs|QDir::NoDotAndDotDot);
        const QStringList systemList = QDir(QString("%1/system/").arg(strMANUAL_DIR)).entryList(QDir::Dirs|QDir::NoDotAndDotDot);
        QString oldMdPath = strMANUAL_DIR;
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
        }

        // 非应用文档，直接添加
        if (systemList.contains(kLearnBasicOperations) || oldAppList.contains(kLearnBasicOperations)) {
            if (app_list_.indexOf(kLearnBasicOperations) == -1)
                app_list_.append(kLearnBasicOperations);
        }
        if (systemList.contains(kCommonApplicationLibraries) || oldAppList.contains(kCommonApplicationLibraries)) {
            if (app_list_.indexOf(kCommonApplicationLibraries) == -1)
                app_list_.append(kCommonApplicationLibraries);
        }
        qDebug() << "exist app list: " << app_list_ << ", count:" << app_list_.size();
    }
    qDebug() << "exist app list: " << app_list_ << ", count:" << app_list_.size();
    return app_list_;
}

/**
 * @brief Utils::getSystemManualDir
 * @return
 * @note　获取系统版本信息
 */
QStringList Utils::getSystemManualDir()
{
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
    QMapIterator<qlonglong, AppInfo> it(map);
    QList<AppInfo> listEnd;
    QList<AppInfo> listtmp;
    qlonglong longlongtmp = 0;
    while (it.hasNext()) {
        it.next();
        //只在第一次循环时插入listtemp
        if (it.value().key == map.first().key) {
            listtmp.append(it.value());
            longlongtmp = it.key();
            continue;
        }

        //如果这本次key与longlongtemp相等说明，当前应用的安装时间与上一次循环中的应用安装时间相同，把appInfo插入listtemp等待排序
        //如果不相等，listtemp不为null，则对littemp按“应用包名”进行排序；
        //并把排序结果添加到listEnd;
        //清空listtmp, 修改longlongtmp记录当前key;
        if (it.key() == longlongtmp) {
            listtmp.append(it.value());
        } else if (listtmp.size() != 0 && it.key() != longlongtmp) {
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
        }
    }
    //最后判断listtmp是否为空，处理循环结束时，最后几次longlongtmp都是相等的情况
    if (!listtmp.isEmpty()) {
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
    return listEnd;
}

/**
 * @brief Utils::hasSelperSupport
 * @return
 * @note 判断是否需要显示服务与支持功能
 */
bool Utils::hasSelperSupport()
{
    Dtk::Core::DSysInfo::UosEdition type = Dtk::Core::DSysInfo::uosEditionType();
    bool bSelperSupport = true;
#ifdef DTKCORE_CLASS_DConfigFile
    DConfig *dconfig = DConfig::create("org.deepin.manual", "org.deepin.manual.customui");
    if (dconfig && dconfig->isValid()&& dconfig->keyList().contains("selperSupport")) {
       bSelperSupport = dconfig->value("selperSupport").toBool();
    }
#endif
    //专业版判断是否有服务与支持
    if (Dtk::Core::DSysInfo::UosProfessional == type || Dtk::Core::DSysInfo::UosMilitary == type || Dtk::Core::DSysInfo::UosMilitaryS == type) {
        const QStringList list = getSystemManualList();
        if (list.contains("uos-service-support") && bSelperSupport) {
            return true;
        }
    }
    return false;
}

bool Utils::hasAppStore()
{
    const QStringList list = getSystemManualList();
    if (list.contains("deepin-app-store")) {
        return true;
    } else {
        return false;
    }
}

//p表示桌面专业版，h表示个人版，d表示社区版
//s表示默认服务器版，e表示服务器企业版，eu表示服务器欧拉版，i表示服务器行业版，klu表示KelvinU项目版本，pgv表示PanguV项目版本。
QStringList Utils::systemToOmit(Dtk::Core::DSysInfo::UosEdition type)
{
    QStringList retList;
    switch (type) {
    //专业版
    case  Dtk::Core::DSysInfo::UosProfessional:
    case  Dtk::Core::DSysInfo::UosMilitary:
#if (DTK_VERSION >= DTK_VERSION_CHECK(5, 6, 11, 1))
    case Dtk::Core::DSysInfo::UosPersonal:
#endif
        retList.append("p");
        break;
    //个人版
    case  Dtk::Core::DSysInfo::UosHome:
        retList.append("h");
        break;
    //社区版
    case  Dtk::Core::DSysInfo::UosCommunity:
        retList.append("d");
        break;
    //服务器企业版
    case  Dtk::Core::DSysInfo::UosEnterprise:
    case  Dtk::Core::DSysInfo::UosMilitaryS:
        retList.append("e");
        retList.append("s");
        break;
    //服务器行业版
    case  Dtk::Core::DSysInfo::UosEnterpriseC:
        retList.append("i");
        retList.append("s");
        break;
    //服务器欧拉版
    case  Dtk::Core::DSysInfo::UosEuler:
        retList.append("eu");
        retList.append("s");
        break;
    //教育版
    case Dtk::Core::DSysInfo::UosEducation:
        retList.append("edu");
        break;
    default:
        break;
    }
    return retList;
}

bool Utils::activeWindow(quintptr winId)
{
    bool bsuccess = true;
    // new interface use applicationName as id
    QDBusInterface manual("com.deepin.dde.daemon.Dock", "/com/deepin/dde/daemon/Dock",
                          "com.deepin.dde.daemon.Dock");
    QDBusReply<void> reply = manual.call("ActivateWindow", winId);
    if (!reply.isValid()) {
        qDebug() << "call com.deepin.dde.daemon.Dock failed" << reply.error();
        bsuccess = false;
    }
    return bsuccess;
}

QString Utils::regexp_label(const QString &strtext, const QString &strpatter)
{
    QString strsource = strtext;
    QString result;
    QRegularExpression re(strpatter, QRegularExpression::MultilineOption | QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = re.match(strsource);
    if (match.isValid() && match.hasMatch()) {
        for (int i = 0; i <= match.lastCapturedIndex(); i++) {
            result = match.captured(i);
            qDebug() << __FUNCTION__ << "-------****" << result;
            break;
        }
    }
    return result;
}

QString Utils::mkMutiDir(const QString &path)
{
    QDir dir(path);
    if (path.isEmpty() || dir.exists()) {
        return path;
    }
    QString parentDir = mkMutiDir(path.mid(0, path.lastIndexOf('/')));
    QString dirname = path.mid(path.lastIndexOf('/') + 1);
    QDir parentPath(parentDir);
    if (!dirname.isEmpty()) {
        bool ret = parentPath.mkpath(dirname);
        qDebug() << "mkpath result:" << ret << dirname;
    }
    return parentDir + "/" + dirname;
}

/**
 * @brief Utils::judgeLongson
 * @node 判断龙芯
 * @return
 */
bool Utils::judgeLoongson()
{
    if (cpuModeName.isEmpty()) {
        QProcess process;
        //获取CPU型号
        process.start("cat /proc/cpuinfo");

        if (process.waitForFinished()) {
            QString result = process.readAllStandardOutput();

            if (result.contains("Loongson")) {
                qWarning() << "cpu mode name is loongson";
                cpuModeName = "Loongson";
            } else {
                cpuModeName = "other";
            }
        }
    }

    if (cpuModeName.contains("Loongson")) {
        return  true;
    }

    return false;
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
    DPalette pa;
    const QColor *qcolor_list, *dcolor_list;

    if (type == DarkType) {
        qcolor_list = dark_qpalette;
        dcolor_list = dark_dpalette;
    } else {
        qcolor_list = light_qpalette;
        dcolor_list = light_dpalette;
    }

    for (int i = 0; i < DPalette::NColorRoles; ++i) {
        QPalette::ColorRole role = static_cast<QPalette::ColorRole>(i);

        QColor color = qcolor_list[i];
        pa.setColor(DPalette::Active, role, color);
        generatePaletteColor(pa, role, type);
    }

    for (int i = 0; i < DPalette::NColorTypes; ++i) {
        DPalette::ColorType role = static_cast<DPalette::ColorType>(i);

        QColor color = dcolor_list[i];
        pa.setColor(DPalette::Active, role, color);
        generatePaletteColor(pa, role, type);
    }

    return pa;
}

DPalette ExApplicationHelper::palette(const QWidget *widget, const QPalette &base) const
{
    Q_UNUSED(base)

    DPalette palette;

    // 存在自定义palette时应该根据其自定义的palette获取对应色调的DPalette
    const QPalette &wp = widget->palette();

    palette = standardPalette(toColorType(wp));

    // 关注控件palette改变的事件
    const_cast<QWidget *>(widget)->installEventFilter(const_cast<ExApplicationHelper *>(this));

    return palette;
}

void ExApplicationHelper::setPalette(QWidget *widget, const DPalette &palette)
{
    // 记录此控件被设置过palette
    widget->setProperty("_d_set_palette", true);
    widget->setPalette(palette);
}

ExApplicationHelper::ExApplicationHelper()
{
}

ExApplicationHelper::~ExApplicationHelper()
{
}

bool ExApplicationHelper::eventFilter(QObject *watched, QEvent *event)
{
    return DGuiApplicationHelper::eventFilter(watched, event);
}
