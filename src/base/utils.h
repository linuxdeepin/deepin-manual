// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef UTILS_H
#define UTILS_H

#include "sys/time.h"
#include "dbus/dbusvariant/app_info.h"

#include <DApplicationHelper>
#include <DSysInfo>
#include <QWidget>

#define dApp (static_cast<DApplication *>(QCoreApplication::instance()))

DWIDGET_USE_NAMESPACE

class Utils : public QObject
{
    Q_OBJECT

public:
    explicit Utils(QObject *parent = nullptr);
    ~Utils();

    enum FontType { SourceHanSansMedium,
                    SourceHanSansNormal,
                    DefautFont
                  };

    static QHash<QString, QPixmap> m_imgCacheHash;
    static QHash<QString, QString> m_fontNameCache;
    static QString cpuModeName;
    static QPixmap renderSVG(const QString &filePath, const QSize &size);
    static QString translateTitle(const QString &titleUS);
    //获取所有应用列表
    static QList<AppInfo> launcherInterface();
    //获取系统应用中有帮助手册的应用列表
    static QStringList getSystemManualList();
    static QStringList getSystemManualDir();
    static QList<AppInfo> sortAppList(QMultiMap<qlonglong, AppInfo> map);
    static bool hasSelperSupport();
    static QStringList systemToOmit(Dtk::Core::DSysInfo::UosEdition);
    static bool activeWindow(quintptr winId);
    static QString regexp_label(const QString &strtext, const QString &strpatter);
    //创建多级目录
    static QString mkMutiDir(const QString &path);
    //判断是否为龙芯平台
    static bool judgeLoongson();
    //判断是否Wayland
    static bool judgeWayLand();
    //判断是否为玲珑平台
    static bool judgeLingLong();
    //获取md文件路径
    static QStringList getMdsourcePath();
    //获取环境变量
    static QStringList getEnvsourcePath();
};

class ExApplicationHelper : public DGuiApplicationHelper
{
    Q_OBJECT

public:
    static ExApplicationHelper *instance();

    DPalette standardPalette(DGuiApplicationHelper::ColorType type) const;
    DPalette palette(const QWidget *widget, const QPalette &base = QPalette()) const;
    void setPalette(QWidget *widget, const DPalette &palette);

private:
    ExApplicationHelper();
    ~ExApplicationHelper() override;

    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif
