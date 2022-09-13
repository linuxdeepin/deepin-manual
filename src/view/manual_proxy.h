// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEEPIN_MANUAL_VIEW_MANUAL_PROXY_H
#define DEEPIN_MANUAL_VIEW_MANUAL_PROXY_H

#include <QObject>

class QIconLoader;
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
    //发送页面加载完成时间
    void startFinish(qint64);
    void updateLabel();
    void iconThemeChanged(const QString &iconTheme);
    void languageChanged();

public slots:
    QString getSystemManualDir();
    QStringList getSystemManualList();

    void setApplicationState(const QString &appName);
    QStringList getUsedAppList();
    bool hasSelperSupport();
    void finishChannel();
    void supportClick();
    bool bIsLongSon();
    void showUpdateLabel();
    QString appToPath(const QString &appName);
    QString getAppIconPath(const QString &desktopname);
    QString getLocalAppName(const QString &desktopname);

    QString translateTitle(const QString &titleUS);

private:
    void saveAppList(const QStringList &list);
    QString getAppLocalDir(const QString &appPath);

private:
    QStringList app_list_;
    QString strIconTheme;
    QIconLoader *piconload;
};

#endif // DEEPIN_MANUAL_VIEW_MANUAL_PROXY_H
