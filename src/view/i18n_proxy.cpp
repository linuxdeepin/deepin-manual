// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
        {"System", QObject::tr("System")},
        {"Applications", QObject::tr("Applications")},
        //      { "NoResult",QObject::tr(
        //                      "Sorry, there are no search results for \"%1\"") },
        {"NoResult", QObject::tr("No search results")},
        //      { "WikiSearch", QObject::tr(
        //          "Change your keywords and try again, or search for it in Deepin Wiki") },
        {"ToIndexPage", QObject::tr("Home")},
        //      { "SearchInWiki", QObject::tr("Deepin Wiki") },
        {"ResultNumSuffix", QObject::tr("  result")}, //单数结果后缀
        {"ResultNumSuffixs", QObject::tr("  results")}};
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
