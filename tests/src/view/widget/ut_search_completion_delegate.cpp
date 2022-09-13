// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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


TEST_F(ut_search_completion_delegate_test, paint2)
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
    sv.setItemDelegate(sd);
    QPainter *painter = new QPainter;
    const QModelIndex index = sv.model()->index(0, 0);
    QStyleOptionViewItem  option;
//    const QModelIndex index;

    option.state = QStyle::State_Selected;
    //    Stub s;
    //    s.set(ADDR(QModelIndex, isValid), stub_true);

    sd->paint(painter, option, index);
    QPixmap pix = sv.grab(QRect(QPoint(0, 0), QSize(0, 0)));

    // s.reset(ADDR(QModelIndex, isValid));

    option.state = QStyle::State_On;
    sd->paint(painter, option, index);

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
    ASSERT_TRUE(sizehit.height() > 0);

    s.reset(ADDR(QModelIndex, isValid));
    delete painter;
}
