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
#include "ut_search_completion_listview_test.h"


#define private public
#define protected public
#include "view/widget/search_completion_listview.h"

#undef private
#undef protected


ut_search_completion_listview_test::ut_search_completion_listview_test()
{

}

TEST_F(ut_search_completion_listview_test, initDelegate)
{
    SearchCompletionListView sv;
    sv.initDelegate();
    ASSERT_TRUE(sv.m_searchCompletionDelegate);
}

TEST_F(ut_search_completion_listview_test, getErrorListSourceModel)
{
    SearchCompletionListView sv;
    QStandardItemModel *model = sv.getErrorListSourceModel();
    ASSERT_EQ(model, sv.m_searchCompletionSourceModel);
}

TEST_F(ut_search_completion_listview_test, mousePressEvent)
{
    QMouseEvent *event;
    SearchCompletionListView sv;
    sv.mousePressEvent(event);
}

TEST_F(ut_search_completion_listview_test, mousePressEvent2)
{
    QMouseEvent *event;
    SearchCompletionListView sv;
    QTest::mouseClick(sv.window(), Qt::LeftButton);
    ASSERT_TRUE(sv.m_bLeftMouse);
    QTest::mouseClick(sv.window(), Qt::RightButton);
    ASSERT_FALSE(sv.m_bLeftMouse);
}

TEST_F(ut_search_completion_listview_test, setSelection)
{

    SearchCompletionListView sv;
    QRect rect;
    QItemSelectionModel::SelectionFlags command;
    sv.m_bLeftMouse = true;
    sv.SearchCompletionListView::setSelection(rect, command);

}

TEST_F(ut_search_completion_listview_test, paintEvent)
{
    SearchCompletionListView sv;
    QPaintEvent *event;
    sv.paintEvent(event);
}
