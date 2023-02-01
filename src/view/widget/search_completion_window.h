// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DEEPIN_MANUAL_VIEW_SEARCH_COMPLETION_WINDOW_H
#define DEEPIN_MANUAL_VIEW_SEARCH_COMPLETION_WINDOW_H

#include "controller/search_result.h"
#include "search_completion_listview.h"

#include <DBlurEffectWidget>

DWIDGET_USE_NAMESPACE

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

#endif // DEEPIN_MANUAL_VIEW_SEARCH_COMPLETION_WINDOW_H
