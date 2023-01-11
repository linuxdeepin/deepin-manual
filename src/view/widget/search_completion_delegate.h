// SPDX-FileCopyrightText: 2019 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef SEARCH_COMPLETION_DELEGATE_H
#define SEARCH_COMPLETION_DELEGATE_H

#include <DStyledItemDelegate>

DWIDGET_USE_NAMESPACE

struct SearchCompletionItemModel {
    QString strSearchAnchorId;
    QString strSearchKeyword;
    QString strSearchAppName;
    QString strSearchAppDisplayName;
};

Q_DECLARE_METATYPE(SearchCompletionItemModel)

class SearchCompletionDelegate : public DStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SearchCompletionDelegate(QAbstractItemView *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

private:
    QAbstractItemView *m_parentView;
};

#endif // SEARCH_COMPLETION_DELEGATE_H
