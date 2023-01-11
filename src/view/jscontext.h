// SPDX-FileCopyrightText: 2019 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef JSCONTEXT_H
#define JSCONTEXT_H

#include <QObject>

class JsContext : public QObject
{
    Q_OBJECT
public:
    explicit JsContext(QObject *parent = nullptr);

signals:
    void parseMsg(QString msg, QString path);

public slots:
    void recvParseMsg(const QString &msg, const QString &path);
};

#endif // JSCONTEXT_H
