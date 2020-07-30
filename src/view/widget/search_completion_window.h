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

#ifndef DEEPIN_MANUAL_VIEW_SEARCH_COMPLETION_WINDOW_H
#define DEEPIN_MANUAL_VIEW_SEARCH_COMPLETION_WINDOW_H

#include "controller/search_result.h"
#include "search_completion_listview.h"

#include <DBlurEffectWidget>

DWIDGET_USE_NAMESPACE

namespace dman {

class SearchButton;

/**
 * @brief The SearchCompletionWindow class 搜索下拉框
 * 由SearchCompletionListView与SearchButton两个类组合成的窗体
 * 搜索结果显示在SearchEdit下方
 */
class SearchCompletionWindow : public DBlurEffectWidget
{
    Q_OBJECT
public:
    explicit SearchCompletionWindow(QWidget *parent = nullptr);
    ~SearchCompletionWindow() override;
    void updateColor(const QColor &color);

    void autoResize();
    const QString &keyword() const
    {
        return keyword_;
    }

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void resultClicked(const SearchAnchorResult &result);
    void searchButtonClicked();

public slots:
    void goDown();
    void goUp();
    void onEnterPressed();

    void setKeyword(const QString &keyword);
    void setSearchAnchorResult(const SearchAnchorResultList &result);

private:
    void initConnections();
    void initSearchCompletionListData(QList<SearchCompletionItemModel> dataList);
    void initUI();

    SearchCompletionListView *result_view_ {nullptr};
    QStandardItemModel *search_compeletion_model_ {nullptr};
    SearchButton *search_button_ {nullptr};
    SearchAnchorResultList result_;
    QString keyword_;

private slots:
    void onSearchButtonEntered();
    void onResultListClicked(const QModelIndex &index);
    void onResultListEntered(const QModelIndex &index);
};

} // namespace dman

#endif // DEEPIN_MANUAL_VIEW_SEARCH_COMPLETION_WINDOW_H
