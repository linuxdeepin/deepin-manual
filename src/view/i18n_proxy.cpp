// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "view/i18n_proxy.h"

#include <QLocale>

I18nProxy::I18nProxy(QObject *parent)
    : QObject(parent)
{
}

I18nProxy::~I18nProxy()
{
}

QVariantHash I18nProxy::getSentences() const
{
    QVariantHash result {
        {"QuickStart", QObject::tr("Quick Start")},
        {"VideoGuide", QObject::tr("Video Guide")},
        {"System", QObject::tr("System")},
        {"Applications", QObject::tr("Applications")},
        {"NoResult", QObject::tr("No search results")},
        {"ToIndexPage", QObject::tr("Home")},
        {"ResultNumSuffix", QObject::tr("  result")}, //单数结果后缀
        {"ResultNumSuffixs", QObject::tr("  results")},
        {"ClicktoView", QObject::tr("Click to view ")},
        {"ViewAll", QObject::tr("View all")},
        {"Support", QObject::tr("Support")},
        {"AppStore", QObject::tr("App Store")}
    };

    return result;
}

QString I18nProxy::getLocale() const
{
    const QString locale = QLocale().name();
    // Fallback to default locale.
//    if (locale != "en_US" && locale != "zh_CN") {
//        return "en_US";
//    } else {
//        return locale;
//    }
    if (locale == "zh_CN" || locale == "zh_HK" || locale == "zh_TW") {
        return "zh_CN";
    } else if (locale == "en_US" || locale == "en_GB") {
        return "en_US";
    } else {
        return  "";
    }
}
