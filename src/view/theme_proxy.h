// SPDX-FileCopyrightText: 2019 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef THEME_PROXY_H
#define THEME_PROXY_H

#include <QObject>

/**
 * @brief The ThemeProxy class
 * 主题接口类，使帮助手册主题色跟随系统颜色变化，ｊｓ调用
 */
class ThemeProxy : public QObject
{
    Q_OBJECT
public:
    explicit ThemeProxy(QObject *parent = nullptr);
    ~ThemeProxy() override;
signals:
    void themeChange(const QString &themetype);
public slots:
    void slot_ThemeChange();
    QString getTheme() const;
};
#endif // THEME_PROXY_H
