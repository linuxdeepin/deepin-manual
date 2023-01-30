// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
