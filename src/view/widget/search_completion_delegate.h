/*
   * Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
   *
   * Author:     wangmingliang <wangmingliang@uniontech.com>
   *
   * Maintainer: wangmingliang <wangmingliang@uniontech.com>
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

#ifndef SEARCH_COMPLETION_DELEGATE_H
#define SEARCH_COMPLETION_DELEGATE_H

#include <DStyledItemDelegate>
#include <QPainterPath>

DWIDGET_USE_NAMESPACE

struct SearchCompletionItemModel {
    QString strSearchAnchorId;
    QString strSearchKeyword;
    QString strSearchAppName;
    QString strSearchAppDisplayName;

    SearchCompletionItemModel()
    {
        strSearchAnchorId = "";
        strSearchKeyword = "";
        strSearchAppName = "";
        strSearchAppDisplayName = "";
    }
};

Q_DECLARE_METATYPE(SearchCompletionItemModel)

class SearchCompletionDelegate : public DStyledItemDelegate
{
    Q_OBJECT
public:
    SearchCompletionDelegate(QAbstractItemView *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

private:
    QAbstractItemView *m_parentView;
};

#endif // SEARCH_COMPLETION_DELEGATE_H
