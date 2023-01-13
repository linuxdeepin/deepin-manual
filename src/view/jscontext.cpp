// SPDX-FileCopyrightText: 2019 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "jscontext.h"

#include <QDebug>

JsContext::JsContext(QObject *parent): QObject(parent)
{

}

void JsContext::recvParseMsg(const QString &msg, const QString &path)
{
    qDebug() << "========>" << msg;
    emit parseMsg(msg, path);
}
