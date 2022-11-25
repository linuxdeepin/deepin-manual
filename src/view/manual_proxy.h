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
    QStringList getSystemManualDir();
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
