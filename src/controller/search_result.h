// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DEEPIN_MANUAL_CONTROLLER_SEARCH_ENTRY_H
#define DEEPIN_MANUAL_CONTROLLER_SEARCH_ENTRY_H

#include <QList>

struct SearchAnchorResult {
    QString app_name;               //app name(for system)
    QString app_display_name;       //display name(for users)
    QString anchor;
    QString anchorId;
};
typedef QList<SearchAnchorResult> SearchAnchorResultList;

struct SearchContentResult {
    QString app_name;
    QString anchor;
    QString anchorId;
};

typedef QList<SearchContentResult> SearchContentResultList;

#endif // DEEPIN_MANUAL_CONTROLLER_SEARCH_ENTRY_H
