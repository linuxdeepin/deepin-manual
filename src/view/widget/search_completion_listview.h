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

#ifndef SEARCH_COMPLETION_LISTVIEW_H
#define SEARCH_COMPLETION_LISTVIEW_H

#include "search_completion_delegate.h"

#include <DCheckBox>
#include <DLabel>
#include <DListView>

#include <QMetaType>

DWIDGET_USE_NAMESPACE

class SearchCompletionListView : public DListView
{
    Q_OBJECT

public:
    explicit SearchCompletionListView(QWidget *parent = nullptr);
    ~SearchCompletionListView() override;

    void mousePressEvent(QMouseEvent *event) override;
    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command) override;
    void initDelegate();

    QStandardItemModel *getErrorListSourceModel();

protected:
    void paintEvent(QPaintEvent *event) override;

private:

    SearchCompletionDelegate *m_searchCompletionDelegate;
    QStandardItemModel *m_searchCompletionSourceModel;
    QList<SearchCompletionItemModel> m_searchCompletionModelList;

    bool m_bLeftMouse;

signals:
    void onClickSearchCompletionItem(QModelIndex index);

private slots:

};

#endif // SEARCH_COMPLETION_LISTVIEW_H
