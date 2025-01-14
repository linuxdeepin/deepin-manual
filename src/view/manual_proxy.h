// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEEPIN_MANUAL_VIEW_MANUAL_PROXY_H
#define DEEPIN_MANUAL_VIEW_MANUAL_PROXY_H

#include <QObject>

class ManualProxy : public QObject
{
    Q_OBJECT
public:
    explicit ManualProxy(QObject *parent = nullptr);
    ~ManualProxy() override;
signals:
    void WidgetLower();
    void channelInit();
    void searchEditTextisEmpty();
    void supportBeClick();
    void appStoreBeClick();
    //发送页面加载完成时间
    void startFinish(qint64);
    void updateLabel();
    void iconThemeChanged(const QString &iconTheme);
    void languageChanged();

public slots:
    QStringList getSystemManualDir();
    QStringList getSystemManualList();

    void setApplicationState(const QString &appName);
    QStringList getUsedAppList();
    bool hasSelperSupport();
    bool hasAppStore();
    void finishChannel();
    void supportClick();
    void appStoreClick();
    bool bIsLongSon();
    void showUpdateLabel();
    QString appToPath(const QString &appName);
    QString getAppIconPath(const QString &desktopname);
    QString getLocalAppName(const QString &desktopname);

    QString translateTitle(const QString &titleUS);

    QVariant getVideoGuideInfo();
    void openVideo(QString url = "");

private:
    void saveAppList(const QStringList &list);
    QString getAppLocalDir(const QString &appPath);

private:
    QStringList app_list_;
    QString strIconTheme;
    QString videoUrl;
};

#endif // DEEPIN_MANUAL_VIEW_MANUAL_PROXY_H
