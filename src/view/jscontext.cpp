// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "jscontext.h"

#include <QDebug>

JsContext::JsContext(QObject *parent): QObject(parent)
{

}

void JsContext::recvParseMsg(const QString &msg, const QString &path)
{
    emit parseMsg(msg, path);
}
