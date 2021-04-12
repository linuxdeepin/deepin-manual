/*
* Copyright (C) 2019 ~ 2020 Deepin Technology Co., Ltd.
*
* Author:     wangmingliang <wangmingliang@uniontech.com>
* Maintainer: wangmingliang <wanmgmingliang@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ut_search_completion_delegate.h"

#include "view/widget/search_completion_delegate.h"

#include "view/widget/search_completion_listview.h"
#include "controller/search_result.h"
#include "src/third-party/stub/stub.h"

#include <QPainter>

ut_search_completion_delegate_test::ut_search_completion_delegate_test()
{

}

bool stub_true()
{
    return true;
}

TEST_F(ut_search_completion_delegate_test, paint)
{
    SearchAnchorResultList list;
    for (int i = 0; i < 8; ++i) {
        SearchAnchorResult result;
        result.anchor = QString("运行应用商店") + QString::number(i);
        result.anchorId = "h0";
        result.app_name = "deepin-app-store";
        result.app_display_name = "应用商店";
        list.append(result);
    }
    qDebug() << "ut_search_completion_delegate_test. paint list.size-->" << list.size();

    QList<SearchCompletionItemModel> dataList;
    for (const SearchAnchorResult &entry : list) {
        SearchCompletionItemModel model;
        model.strSearchKeyword = entry.anchor;
        model.strSearchAnchorId = entry.anchorId;
        model.strSearchAppName = entry.app_name;
        model.strSearchAppDisplayName = entry.app_display_name;
        dataList.append(model);
    }

    QStandardItemModel  search_compeletion_model_;// = new QStandardItemModel(result_view_);

    for (int i = 0; i < dataList.size(); i++) {
        QStandardItem *item = new QStandardItem;
        SearchCompletionItemModel itemModel = dataList.at(i);
        item->setData(QVariant::fromValue(itemModel), Qt::DisplayRole);

        search_compeletion_model_.appendRow(item);
    }

    SearchCompletionListView sv;
    sv.setModel(&search_compeletion_model_);

    SearchCompletionDelegate *sd = new SearchCompletionDelegate(&sv);
//    sv.setItemDelegate(sd);
    QPainter *painter = new QPainter;
    const QModelIndex index =  sv.model()->index(1, 1);
    const QStyleOptionViewItem  option;
//    const QModelIndex index;

    Stub s;
    s.set(ADDR(QModelIndex, isValid), stub_true);

    sd->paint(painter, option, index);
    s.reset(ADDR(QModelIndex, isValid));
    delete painter;
}

TEST_F(ut_search_completion_delegate_test, sizeHint)
{
    SearchAnchorResultList list;
    for (int i = 0; i < 8; ++i) {
        SearchAnchorResult result;
        result.anchor = QString("运行应用商店") + QString::number(i);
        result.anchorId = "h0";
        result.app_name = "deepin-app-store";
        result.app_display_name = "应用商店";
        list.append(result);
    }
    qDebug() << "ut_search_completion_delegate_test. paint list.size-->" << list.size();

    QList<SearchCompletionItemModel> dataList;
    for (const SearchAnchorResult &entry : list) {
        SearchCompletionItemModel model;
        model.strSearchKeyword = entry.anchor;
        model.strSearchAnchorId = entry.anchorId;
        model.strSearchAppName = entry.app_name;
        model.strSearchAppDisplayName = entry.app_display_name;
        dataList.append(model);
    }

    QStandardItemModel  search_compeletion_model_;// = new QStandardItemModel(result_view_);

    for (int i = 0; i < dataList.size(); i++) {
        QStandardItem *item = new QStandardItem;
        SearchCompletionItemModel itemModel = dataList.at(i);
        item->setData(QVariant::fromValue(itemModel), Qt::DisplayRole);

        search_compeletion_model_.appendRow(item);
    }

    SearchCompletionListView sv;
    sv.setModel(&search_compeletion_model_);

    SearchCompletionDelegate *sd = new SearchCompletionDelegate(&sv);
//    sv.setItemDelegate(sd);
    QPainter *painter = new QPainter;
    const QModelIndex index =  sv.model()->index(1, 1);
    const QStyleOptionViewItem  option;

    Stub s;
    s.set(ADDR(QModelIndex, isValid), stub_true);

    sd->paint(painter, option, index);
    QSize sizehit = sd->sizeHint(option, index);

    s.reset(ADDR(QModelIndex, isValid));
    delete painter;
}
