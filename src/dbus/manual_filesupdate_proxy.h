// SPDX-FileCopyrightText: 2019 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef MANUAL_FILESUPDATE_PROXY_H
#define MANUAL_FILESUPDATE_PROXY_H

#include <QObject>

class ManualFilesUpdateProxy : public QObject
{
    Q_OBJECT
public:
    explicit ManualFilesUpdateProxy(QObject *parent = nullptr);
    ~ManualFilesUpdateProxy() override;

signals:
    void FilesUpdate(const QStringList &list);
public slots:
    void OnFilesUpdate(const QStringList &list);
};

#endif // MANUAL_FILESUPDATE_PROXY_H
