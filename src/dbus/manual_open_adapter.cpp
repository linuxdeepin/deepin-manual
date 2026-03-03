// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "manual_open_adapter.h"

/**
 * @brief ManualOpenAdapter::ManualOpenAdapter
 * @param parent\
 * @note Implementation of adaptor class ManualOpenAdapter
 */
ManualOpenAdapter::ManualOpenAdapter(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}

ManualOpenAdapter::~ManualOpenAdapter()
{
}

/**
 * @brief ManualOpenAdapter::Open
 * @param in0
 */
void ManualOpenAdapter::Open(const QString &in0)
{
    // 调用 com.deepin.Manual.Open.Open
    QMetaObject::invokeMethod(parent(), "Open", Q_ARG(QString, in0));
}

/**
 * @brief ManualOpenAdapter::OpenTitle
 * @param in0
 * @param in1
 */
void ManualOpenAdapter::OpenTitle(const QString &in0, const QString &in1)
{
    QMetaObject::invokeMethod(parent(), "OpenTitle", Q_ARG(QString, in0), Q_ARG(QString, in1));
}


/**
 * @brief ManualOpenAdapter::Search
 * @param keyword
 */
void ManualOpenAdapter::Search(const QString &keyword)
{
    // 调用 com.deepin.Manual.Open.Search
    QMetaObject::invokeMethod(parent(), "Search", Q_ARG(QString, keyword));
}

/**
 * @brief ManualOpenAdapter::ShowManual
 * @param in0
 */
void ManualOpenAdapter::ShowManual(const QString &in0)
{
    // 调用 com.deepin.Manual.Open.ShowManual
    QMetaObject::invokeMethod(parent(), "ShowManual", Q_ARG(QString, in0));
}
