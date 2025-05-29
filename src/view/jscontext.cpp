// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "jscontext.h"
#include "base/ddlog.h"

#include <QDebug>

JsContext::JsContext(QObject *parent): QObject(parent)
{
    qCDebug(app) << "JsContext constructor called";
}

void JsContext::recvParseMsg(const QString &msg, const QString &path)
{
    qCDebug(app) << "Received message to parse:" << msg << "from path:" << path;
    emit parseMsg(msg, path);
}
