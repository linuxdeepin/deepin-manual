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

#ifndef DEEPIN_MANUAL_VIEW_WIDGETS_SEARCH_BUTTON_H
#define DEEPIN_MANUAL_VIEW_WIDGETS_SEARCH_BUTTON_H

#include <DLabel>
#include <DIconButton>
#include <DBlurEffectWidget>
#include <DApplicationHelper>

DWIDGET_USE_NAMESPACE

/**
* @brief The SearchButton class　全文搜索按钮
* Overloading of push button which emits entered() signal on mouse hover.
* 根据给定关键字全文搜索按钮，位置在搜索提示窗口最下部
* 继承DBlurEffectWidget　模糊窗口，使窗口效果与search_completion_window窗口效果一致
* 重载窗口事件：鼠标进入、鼠标释放、鼠标悬停事件
*/
class SearchButton : public DBlurEffectWidget
{
    Q_OBJECT
public:
    explicit SearchButton(QWidget *parent = nullptr);
    ~SearchButton() override;

    void updateColor(const QColor &color);
    void setText(QString title);
    void setChecked(bool bChecked);
    bool isChecked();
    void leaveFocus();

signals:
    void pressed();
    void entered();

private slots:
    void onThemeChange(DGuiApplicationHelper::ColorType themeType);

protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    DLabel *m_textLabel;
    DIconButton *iconBtn;
    QColor myColor;

    bool m_bHover;
};

#endif // DEEPIN_MANUAL_VIEW_WIDGETS_SEARCH_BUTTON_H
