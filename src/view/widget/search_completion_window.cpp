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

#include "view/widget/search_completion_window.h"
#include "view/widget/search_button.h"
#include "base/utils.h"

#include <DWindowManagerHelper>

#include <QStylePainter>

namespace dman {

namespace {

const int kItemHeight = 34;

} // namespace

SearchCompletionWindow::SearchCompletionWindow(QWidget *parent)
    : DBlurEffectWidget(parent)
{
    this->setObjectName("SearchCompletionWindow");

    this->initUI();
    this->initConnections();
}

SearchCompletionWindow::~SearchCompletionWindow()
{
}

/**
 * @brief SearchCompletionWindow::updateColor
 * @param color 系统活动色
 * 修改系统活动色，使searchButon背景色与系统活动色一致
 */
void SearchCompletionWindow::updateColor(const QColor &color)
{
    if (search_button_) {
        search_button_->updateColor(color);
    }
}

/**
 * @brief SearchCompletionWindow::autoResize
 * 根据搜索结果的数量自动设置窗口高度，最多显示7行结果，支持滚屏
 * 设置searchButton位置在listview下方
 */
void SearchCompletionWindow::autoResize()
{
    int rowCount = result_view_->model()->rowCount();

    int resultViewHeight = kItemHeight;
    if (rowCount > 0 && rowCount < 7) {
        resultViewHeight = rowCount * kItemHeight + 7;
        result_view_->setFixedHeight(resultViewHeight);
        this->setFixedHeight(resultViewHeight + kItemHeight + 7);
        search_button_->setGeometry(0, resultViewHeight, result_view_->width(), kItemHeight + 7);
    } else {
        resultViewHeight = 7 * kItemHeight + 7;
        result_view_->setFixedHeight(7 * kItemHeight + 7);
        this->setFixedHeight(resultViewHeight + kItemHeight + 7);
        search_button_->setGeometry(0, resultViewHeight, result_view_->width(), kItemHeight + 7);
    }

    result_view_->setFixedWidth(this->width());
    search_button_->setFixedWidth(this->width());
    result_view_->setVisible(search_compeletion_model_->rowCount() > 0);
}

/**
 * @brief SearchCompletionWindow::goDown
 * 方向键“下”，实现可循环滚动效果
 */
void SearchCompletionWindow::goDown()
{
    if (nullptr == search_compeletion_model_) {
        return;
    }

    if (search_compeletion_model_->rowCount() == 0) {
        search_button_->setChecked(true);
    } else {
        if (search_button_->isChecked()) {
            search_button_->setChecked(false);
            const QModelIndex first_idx = search_compeletion_model_->index(0, 0);
            result_view_->setCurrentIndex(first_idx);
        } else {
            const int down_row = result_view_->currentIndex().row() + 1;
            if (down_row >= search_compeletion_model_->rowCount()) {
                search_button_->setChecked(true);
                result_view_->setCurrentIndex(QModelIndex());
            } else {
                const QModelIndex down_idx = search_compeletion_model_->index(down_row, 0);
                result_view_->setCurrentIndex(down_idx);
            }
        }
    }
}

/**
 * @brief SearchCompletionWindow::goUp
 * 方向键“上”，实现可循环滚动效果
 */
void SearchCompletionWindow::goUp()
{
    if (nullptr == search_compeletion_model_) {
        return;
    }
    if (search_compeletion_model_->rowCount() == 0) {
        search_button_->setChecked(true);
    } else {
        if (search_button_->isChecked()) {
            search_button_->setChecked(false);
            // Select last item.
            const QModelIndex idx = search_compeletion_model_->index(search_compeletion_model_->rowCount() - 1, 0);
            result_view_->setCurrentIndex(idx);
        } else {
            const int up_row = result_view_->currentIndex().row() - 1;
            if (up_row < 0) {
                result_view_->setCurrentIndex(QModelIndex());
                search_button_->setChecked(true);
            } else {
                const QModelIndex up_idx = search_compeletion_model_->index(up_row, 0);
                result_view_->setCurrentIndex(up_idx);
            }
        }
    }
}

/**
 * @brief SearchCompletionWindow::onEnterPressed
 * 焦点在searchEdit输入关键字后 点击回车键触发该槽
 * 如果searchButton为选择状态时执行全文搜索
 * 否则通过listViewItem的索引获取选中行的内容，执行页面显示
 */
void SearchCompletionWindow::onEnterPressed()
{
    if (search_button_->isChecked()) {
        emit this->searchButtonClicked();
    } else {
        const QModelIndex idx = result_view_->currentIndex();
        this->onResultListClicked(idx);
    }
    // 点击任何一个Ｉｔｅｍ后都隐藏整个窗口
    this->hide();
}

/**
 * @brief SearchCompletionWindow::setKeyword
 * @param keyword 搜索关键字
 * 设置seearchButton中的搜索关键字
 */
void SearchCompletionWindow::setKeyword(const QString &keyword)
{
    keyword_ = keyword;
    QFontMetrics metrics = search_button_->fontMetrics();
    search_button_->setText(
        metrics.elidedText(
            QObject::tr("Search for \"%1\" in the full text").arg(keyword),
            Qt::ElideRight, 350 - 39));
}

/**
 * @brief SearchCompletionWindow::setSearchAnchorResult
 * @param result 搜索结果内容
 * 数据转换，全文搜索结果数据添加到searchDataList中，调用initSearchCompletionListData()方法，设置ListView;
 */
void SearchCompletionWindow::setSearchAnchorResult(const SearchAnchorResultList &result)
{
    result_ = result;
    QList<SearchCompletionItemModel> searchDataList;

    for (const SearchAnchorResult &entry : result) {
        SearchCompletionItemModel model;
        model.strSearchKeyword = entry.anchor;
        model.strSearchAnchorId = entry.anchorId;
        model.strSearchAppName = entry.app_name;
        model.strSearchAppDisplayName = entry.app_display_name;

        searchDataList.append(model);
    }
    initSearchCompletionListData(searchDataList);
    this->autoResize();
}

/**
 * @brief SearchCompletionWindow::initConnections
 * 信号槽连接
 */
void SearchCompletionWindow::initConnections()
{
    connect(result_view_, &SearchCompletionListView::onClickSearchCompletionItem,
            this, &SearchCompletionWindow::onResultListClicked);
    connect(result_view_, &DListView::entered,
            this, &SearchCompletionWindow::onResultListEntered);
    connect(search_button_, &SearchButton::entered,
            this, &SearchCompletionWindow::onSearchButtonEntered);
    connect(search_button_, &SearchButton::pressed,
            this, &SearchCompletionWindow::searchButtonClicked);
}

/**
 * @brief SearchCompletionWindow::initSearchCompletionListData
 * @param dataList 搜索结果list
 * 全文搜索结果数据，添加到QStandardItemModel中，通过setModel()，设置ListView
 */
void SearchCompletionWindow::initSearchCompletionListData(QList<SearchCompletionItemModel> dataList)
{
    search_compeletion_model_ = new QStandardItemModel(result_view_);

    for (int i = 0; i < dataList.size(); i++) {
        QStandardItem *item = new QStandardItem;
        SearchCompletionItemModel itemModel = dataList.at(i);
        item->setData(QVariant::fromValue(itemModel), Qt::DisplayRole);

        search_compeletion_model_->appendRow(item);
    }

    result_view_->setModel(search_compeletion_model_);
}

/**
 * @brief SearchCompletionWindow::initUI
 * 界面初始化，创建ListView对象
 */
void SearchCompletionWindow::initUI()
{
    setAutoFillBackground(false);

    result_view_ = new SearchCompletionListView(this);

    result_view_->setObjectName("ResultList");
    result_view_->setMouseTracking(true);
    result_view_->setEditTriggers(QListView::NoEditTriggers);
    result_view_->setSelectionMode(QListView::SingleSelection);
    result_view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    result_view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    result_view_->setFocusPolicy(Qt::NoFocus);
    result_view_->setMinimumHeight(kItemHeight);

    search_button_ = new SearchButton(this);
    search_button_->setObjectName("SearchButton");
    search_button_->setFixedHeight(35 + 7);
    search_button_->setText(QObject::tr("Search for \"%1\" in the full text"));
    search_button_->setGeometry(0, 7 * kItemHeight + 7, result_view_->width(), 34 + 7);

    this->setContentsMargins(0, 0, 0, 0);
    this->setMinimumHeight(kItemHeight);
    this->setFixedWidth(350);
    this->setWindowFlags(Qt::FramelessWindowHint
                         | Qt::CustomizeWindowHint
                         | Qt::BypassWindowManagerHint);
    this->setAttribute(Qt::WA_NativeWindow, true);
}

/**
 * @brief SearchCompletionWindow::paintEvent
 * @param event
 * 重写绘制事件
 */
void SearchCompletionWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QStylePainter painter(this);
    QPainterPath path;
    path.addRect(this->rect());
    DPalette pa = ExApplicationHelper::instance()->palette(this);
    DStyleHelper styleHelper;
    QColor fillColor;
    if (DWindowManagerHelper::instance()->hasComposite()) {
        fillColor = pa.color(DPalette::FrameBorder);
    } else {
        DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
        if (themeType == DGuiApplicationHelper::LightType) {
            fillColor = QColor(255, 255, 255);
        } else if (themeType == DGuiApplicationHelper::DarkType) {
            fillColor = QColor(0, 0, 0);
        }
    }
    painter.fillPath(path, QBrush(fillColor));
}

/**
 * @brief SearchCompletionWindow::onResultListClicked
 * @param index 被选中的ListItem索引
 * listItem被点击或选中后 按回车时调用，
 * 通QModelIndex确定并获取选中的ListItem中的内容，
 * 内容通过信号resultClicked()发给web_window类，通过调用js中的open接口显示页面
 * 如果索引无效时,模拟的searchButton点击执行全文搜索
 */
void SearchCompletionWindow::onResultListClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        const int row = index.row();
        emit this->resultClicked(result_.at(row));
        result_view_->setCurrentIndex(QModelIndex());
    } else {
        // 模拟按钮单击事件。
        emit this->searchButtonClicked();
    }
    this->hide();
}

/**
 * @brief SearchCompletionWindow::onSearchButtonEntered
 * searchButton被选中，修改状态，并设置ListView索引
 */
void SearchCompletionWindow::onSearchButtonEntered()
{
    search_button_->setChecked(true);
    result_view_->setCurrentIndex(QModelIndex());
}

/**
 * @brief SearchCompletionWindow::onResultListEntered
 * @param index
 * 绑定DListView::entered()信号，鼠标移入Item时解发
 */
void SearchCompletionWindow::onResultListEntered(const QModelIndex &index)
{
    result_view_->setCurrentIndex(index);
    search_button_->setChecked(false);
}

} // namespace dman
