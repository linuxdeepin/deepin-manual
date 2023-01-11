// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DEEPIN_MANUAL_VIEW_WIDGET_SEARCH_EDIT_H
#define DEEPIN_MANUAL_VIEW_WIDGET_SEARCH_EDIT_H

#include <DSearchEdit>

/**
 * @brief The SearchEdit class
 * 标题栏中的搜索框，居中显示
 */
class SearchEdit : public Dtk::Widget::DSearchEdit
{
    Q_OBJECT
public:
    explicit SearchEdit(QWidget *parent = nullptr);
    ~SearchEdit() override;

signals:
    void downKeyPressed();
    void enterPressed();
    void upKeyPressed();
    void onClickedClearBtn();

protected:
    void keyPressEvent(QKeyEvent *event) override;
};


#endif // DEEPIN_MANUAL_VIEW_WIDGET_SEARCH_EDIT_H
