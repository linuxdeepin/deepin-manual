// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEEPIN_MANUAL_VIEW_I18N_PROXY_H
#define DEEPIN_MANUAL_VIEW_I18N_PROXY_H

#include <QVariant>

class I18nProxy : public QObject
{
    Q_OBJECT
public:
    explicit I18nProxy(QObject *parent = nullptr);
    ~I18nProxy() override;

public slots:
    QVariantHash getSentences() const;

    QString getLocale() const;
};

#endif // DEEPIN_MANUAL_VIEW_I18N_PROXY_H
