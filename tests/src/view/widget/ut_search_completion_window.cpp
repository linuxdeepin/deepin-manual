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
#include "ut_search_completion_window.h"
#include "view/widget/search_completion_window.h"
#include "view/widget/search_button.h"
#include "view/web_window.h"
#include "base/utils.h"
#include "src/third-party/stub/stub.h"

#include <DWindowManagerHelper>

ut_search_completion_window_test::ut_search_completion_window_test()
{

}

TEST_F(ut_search_completion_window_test, updateColor)
{
    SearchCompletionWindow sw;
    sw.initUI();
    QColor co(255, 255, 255);
    sw.updateColor(co);
    ASSERT_EQ(sw.search_button_->myColor, co);
}

TEST_F(ut_search_completion_window_test, autoResize)
{
    SearchCompletionWindow sw;
    sw.initUI();
    SearchAnchorResultList list;
    SearchAnchorResult result;
    result.anchor = "应用商店";
    result.anchorId = "h0";
    result.app_name = "deepin-app-store";
    result.app_display_name = "应用商店";
    list.append(result);
    SearchAnchorResult result2;
    result2.anchor = "运行应用商店";
    result2.anchorId = "h3";
    result2.app_name = "deepin-app-store";
    result2.app_display_name = "应用商店";
    list.append(result2);

    QList<SearchCompletionItemModel> searchDataList;

    for (const SearchAnchorResult &entry : list) {
        SearchCompletionItemModel model;
        model.strSearchKeyword = entry.anchor;
        model.strSearchAnchorId = entry.anchorId;
        model.strSearchAppName = entry.app_name;
        model.strSearchAppDisplayName = entry.app_display_name;

        searchDataList.append(model);
    }
    sw.initSearchCompletionListData(searchDataList);
    sw.result_view_->setFixedSize(30, 100);
    sw.search_button_->setFixedSize(100, 30);
    sw.autoResize();
    ASSERT_EQ(sw.result_view_->width(), sw.width());
    ASSERT_EQ(sw.search_button_->width(), sw.width());
    ASSERT_EQ(sw.result_view_->height(), 75);
    ASSERT_EQ(sw.height(), 116);
}


TEST_F(ut_search_completion_window_test, autoResize2)
{
    SearchCompletionWindow sw;
    sw.initUI();
    SearchAnchorResultList list;
    for (int i = 0; i < 8; ++i) {
        SearchAnchorResult result;
        result.anchor = QString("运行应用商店") + QString::number(i);
        result.anchorId = "h0";
        result.app_name = "deepin-app-store";
        result.app_display_name = "应用商店";
        list.append(result);
    }
    qDebug() << "searchAnchorResultList.size-->" << list.size();

    QList<SearchCompletionItemModel> searchDataList;
    for (const SearchAnchorResult &entry : list) {
        SearchCompletionItemModel model;
        model.strSearchKeyword = entry.anchor;
        model.strSearchAnchorId = entry.anchorId;
        model.strSearchAppName = entry.app_name;
        model.strSearchAppDisplayName = entry.app_display_name;
        searchDataList.append(model);
    }
    sw.initSearchCompletionListData(searchDataList);
    sw.result_view_->setFixedSize(30, 100);
    sw.search_button_->setFixedSize(100, 30);
    sw.autoResize();
    ASSERT_EQ(sw.result_view_->width(), sw.width());
    ASSERT_EQ(sw.search_button_->width(), sw.width());
    ASSERT_EQ(sw.result_view_->height(), 245);
    ASSERT_EQ(sw.height(), 286);
}

TEST_F(ut_search_completion_window_test, initSearchCompletionListData)
{
    SearchCompletionWindow sw;
    sw.initUI();
    SearchAnchorResultList list;
    for (int i = 0; i < 8; ++i) {
        SearchAnchorResult result;
        result.anchor = QString("运行应用商店") + QString::number(i);
        result.anchorId = "h0";
        result.app_name = "deepin-app-store";
        result.app_display_name = "应用商店";
        list.append(result);
    }
    qDebug() << "searchAnchorResultList.size-->" << list.size();

    QList<SearchCompletionItemModel> searchDataList;
    for (const SearchAnchorResult &entry : list) {
        SearchCompletionItemModel model;
        model.strSearchKeyword = entry.anchor;
        model.strSearchAnchorId = entry.anchorId;
        model.strSearchAppName = entry.app_name;
        model.strSearchAppDisplayName = entry.app_display_name;
        searchDataList.append(model);
    }
    sw.initSearchCompletionListData(searchDataList);
    sw.autoResize();

    sw.goDown();
    QVariant variant = sw.result_view_->currentIndex().data(Qt::DisplayRole);
    SearchCompletionItemModel data = variant.value<SearchCompletionItemModel>();
    QString msg = data.strSearchAppName;

    ASSERT_EQ(msg, "deepin-app-store");

}


TEST_F(ut_search_completion_window_test, initSearchCompletionListData2)
{
    SearchCompletionWindow sw;
    sw.initUI();
    SearchAnchorResultList list;
    for (int i = 0; i < 5; ++i) {
        SearchAnchorResult result;
        result.anchor = QString("运行应用商店") + QString::number(i);
        result.anchorId = "h0";
        result.app_name = "deepin-app-store";
        result.app_display_name = "应用商店";
        list.append(result);
    }
    qDebug() << "searchAnchorResultList.size-->" << list.size();

    QList<SearchCompletionItemModel> searchDataList;
    for (const SearchAnchorResult &entry : list) {
        SearchCompletionItemModel model;
        model.strSearchKeyword = entry.anchor;
        model.strSearchAnchorId = entry.anchorId;
        model.strSearchAppName = entry.app_name;
        model.strSearchAppDisplayName = entry.app_display_name;
        searchDataList.append(model);
    }
    sw.initSearchCompletionListData(searchDataList);
    sw.autoResize();

    sw.goDown();
    QVariant variant = sw.result_view_->currentIndex().data(Qt::DisplayRole);
    SearchCompletionItemModel data = variant.value<SearchCompletionItemModel>();
    QString msg = data.strSearchAppName;

    ASSERT_EQ(msg, "deepin-app-store");

}


TEST_F(ut_search_completion_window_test, goDown)
{
    SearchCompletionWindow sw;
    sw.initUI();
    SearchAnchorResultList list;
    for (int i = 0; i < 3; ++i) {
        SearchAnchorResult result;
        result.anchor = QString("运行应用商店") + QString::number(i);
        result.anchorId = "h0";
        result.app_name = "deepin-app-store";
        result.app_display_name = "应用商店";
        list.append(result);
    }
    sw.setSearchAnchorResult(list);
    qDebug() << "sw.search_compeletion_model_->rowCount() -->" << sw.search_compeletion_model_->rowCount();
    sw.goDown();
    ASSERT_EQ(sw.result_view_->currentIndex().row(), 0);
    sw.goDown();
    ASSERT_EQ(sw.result_view_->currentIndex().row(), 1);
    sw.goDown();
    ASSERT_EQ(sw.result_view_->currentIndex().row(), 2);
    sw.goDown();
    ASSERT_TRUE(sw.search_button_->isChecked());
    sw.goDown();
    ASSERT_FALSE(sw.search_button_->isChecked());
    ASSERT_EQ(sw.result_view_->currentIndex().row(), 0);

    sw.goUp();
    ASSERT_TRUE(sw.search_button_->isChecked());
    sw.goUp();
    ASSERT_EQ(sw.result_view_->currentIndex().row(), 2);
    ASSERT_FALSE(sw.search_button_->isChecked());
}

TEST_F(ut_search_completion_window_test, goDown2)
{
    SearchCompletionWindow sw;
    sw.goDown();
    ASSERT_FALSE(sw.search_compeletion_model_);
    sw.initUI();

    SearchAnchorResultList list;
    sw.setSearchAnchorResult(list);
    sw.goDown();
    ASSERT_TRUE(sw.search_button_->isChecked());
}

TEST_F(ut_search_completion_window_test, goUp)
{
    SearchCompletionWindow sw;
    sw.initUI();
    SearchAnchorResultList list;
    for (int i = 0; i < 3; ++i) {
        SearchAnchorResult result;
        result.anchor = QString("运行应用商店") + QString::number(i);
        result.anchorId = "h0";
        result.app_name = "deepin-app-store";
        result.app_display_name = "应用商店";
        list.append(result);
    }
    sw.setSearchAnchorResult(list);
    qDebug() << "sw.search_compeletion_model_->rowCount() -->" << sw.search_compeletion_model_->rowCount();
    sw.goUp();
    ASSERT_TRUE(sw.search_button_->isChecked());
    sw.goUp();
    ASSERT_EQ(sw.result_view_->currentIndex().row(), 2);
    ASSERT_FALSE(sw.search_button_->isChecked());
    sw.goUp();
    ASSERT_EQ(sw.result_view_->currentIndex().row(), 1);
    sw.goUp();
    ASSERT_EQ(sw.result_view_->currentIndex().row(), 0);
    sw.goUp();
    ASSERT_TRUE(sw.search_button_->isChecked());
}

TEST_F(ut_search_completion_window_test, goUp2)
{
    SearchCompletionWindow sw;
    sw.goUp();
    ASSERT_FALSE(sw.search_compeletion_model_);
    sw.initUI();
    SearchAnchorResultList list;
    sw.setSearchAnchorResult(list);
    sw.goUp();
    ASSERT_TRUE(sw.search_button_->isChecked());
}

TEST_F(ut_search_completion_window_test, onEnterPressed)
{
    SearchCompletionWindow sw;
//    sw.initUI();
    SearchAnchorResultList list;
    sw.setSearchAnchorResult(list);
    sw.goUp();
    ASSERT_TRUE(sw.search_button_->isChecked());
    sw.onEnterPressed();
    ASSERT_FALSE(sw.isVisible());
    sw.search_button_->setChecked(false);
    sw.onEnterPressed();
    ASSERT_FALSE(sw.isVisible());
}

bool stub_hasComposite()
{
    return false;
}

DGuiApplicationHelper::ColorType stub_themeType()
{
    return DGuiApplicationHelper::DarkType;
}

TEST_F(ut_search_completion_window_test, paintEvent)
{
    SearchCompletionWindow sw;
    QPaintEvent *event;
    sw.paintEvent(event);
    DPalette pa = ExApplicationHelper::instance()->palette(sw.window());
    QColor fillColor = pa.color(DPalette::FrameBorder);
    if (DWindowManagerHelper::instance()->hasComposite()) {
        ASSERT_EQ(fillColor, pa.color(DPalette::FrameBorder));
    }

    Stub st;
    st.set(ADDR(DWindowManagerHelper, hasComposite), stub_hasComposite);
    sw.paintEvent(event);

    st.set(ADDR(DGuiApplicationHelper, themeType), stub_themeType);
    sw.paintEvent(event);


}

TEST_F(ut_search_completion_window_test, onResultListClicked)
{
    SearchCompletionWindow sw;
    sw.initUI();
    SearchAnchorResultList list;
    for (int i = 0; i < 3; ++i) {
        SearchAnchorResult result;
        result.anchor = QString("运行应用商店") + QString::number(i);
        result.anchorId = "h0";
        result.app_name = "deepin-app-store";
        result.app_display_name = "应用商店";
        list.append(result);
    }
    sw.setSearchAnchorResult(list);
    sw.goDown();
    QModelIndex index = sw.result_view_->currentIndex();
    sw.onResultListClicked(index);
    ASSERT_FALSE(sw.isVisible());
}

TEST_F(ut_search_completion_window_test, setKeyword)
{
    SearchCompletionWindow sw;
    sw.initUI();
    sw.setKeyword("应用");
    ASSERT_TRUE(sw.search_button_->m_textLabel->text().contains("应用"));
}

TEST_F(ut_search_completion_window_test, onSearchButtonEnterd)
{
    SearchCompletionWindow sw;
    sw.onSearchButtonEntered();
    ASSERT_TRUE(sw.search_button_->isChecked());
}

TEST_F(ut_search_completion_window_test, onResultListEntered)
{
    SearchCompletionWindow sw;
    sw.initUI();
    SearchAnchorResultList list;
    for (int i = 0; i < 3; ++i) {
        SearchAnchorResult result;
        result.anchor = QString("运行应用商店") + QString::number(i);
        result.anchorId = "h0";
        result.app_name = "deepin-app-store";
        result.app_display_name = "应用商店";
        list.append(result);
    }
    sw.setSearchAnchorResult(list);
    sw.goDown();
    QModelIndex index = sw.result_view_->currentIndex();
    sw.onResultListEntered(index);
    ASSERT_EQ(sw.result_view_->currentIndex(), index);
    ASSERT_FALSE(sw.search_button_->isChecked());
}
