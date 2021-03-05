/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
//    if (locale != "en_US" && locale != "es" && locale != "zh_CN") {
//        return "en_US";
//    } else {
//        return locale;
//    }
    if (locale == "zh_CN" || locale == "zh_HK" || locale == "zh_TW") {
        return "zh_CN";
    } else if (locale == "en_US" || locale == "en_GB") {
        return "en_US";
    } else if (locale == "es" || locale == "es_AR" || locale == "es_ES" || locale == "es_419") {
        return "es";
    } else {
        return  "";
    }
}
