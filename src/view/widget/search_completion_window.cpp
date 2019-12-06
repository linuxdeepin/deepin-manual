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
#include "view/theme_manager.h"
#include "view/widget/search_button.h"
#include "base/utils.h"

#include <DLog>
#include <QStylePainter>
#include <QVBoxLayout>

#include <DPlatformWindowHandle>
#include <DStyleHelper>

namespace dman {

namespace {

const int kItemHeight = 34;

}  // namespace

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

void SearchCompletionWindow::autoResize()
{
    int rowCount = result_view_->model()->rowCount();

    int resultViewHeight = kItemHeight;
    if (rowCount > 0 && rowCount < 7) {
        resultViewHeight = rowCount * kItemHeight + 7;
        result_view_->setFixedHeight(resultViewHeight);
        this->setFixedHeight(resultViewHeight + kItemHeight + 7);
        search_button_->setGeometry(0, resultViewHeight, result_view_->width(), kItemHeight+7);
    }
    else {
        resultViewHeight = 7 * kItemHeight + 7;
        result_view_->setFixedHeight(7 * kItemHeight + 7);
        this->setFixedHeight(resultViewHeight + kItemHeight + 7);
        search_button_->setGeometry(0, resultViewHeight, result_view_->width(), kItemHeight+7);
    }

    result_view_->setFixedWidth(this->width());
    search_button_->setFixedWidth(this->width());
    result_view_->setVisible(search_compeletion_model_->rowCount() > 0);
}

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

void SearchCompletionWindow::onEnterPressed()
{
    if (search_button_->isChecked()) {
        emit this->searchButtonClicked();
    } else {
        const QModelIndex idx = result_view_->currentIndex();
        this->onResultListClicked(idx);
    }
    // Hide completion window whenever any items was activated.
    this->hide();
}

void SearchCompletionWindow::setKeyword(const QString &keyword)
{
    keyword_ = keyword;

    QFontMetrics metrics = search_button_->fontMetrics();
    search_button_->setText(
        metrics.elidedText(
//            QObject::tr("Search for \"%1\" in the full text").arg(keyword), //ak问题 4292
            QString("在全文中搜索“%1”").arg(keyword),
            Qt::ElideRight,
            350-39));
}

void SearchCompletionWindow::setSearchAnchorResult(const SearchAnchorResultList &result)
{
    result_ = result;

    QList<SearchCompletionItemModel> searchDataList;
    for (const SearchAnchorResult &entry : result) {
        QString anchor = QString("%1(%2)").arg(entry.anchor).arg(entry.app_display_name);
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

void SearchCompletionWindow::initConnections()
{
    connect(result_view_, &SearchCompletionListView::onClickSearchCompletionItem,
            this, &SearchCompletionWindow::onResultListClicked);
    connect(result_view_, &DListView::entered,
            this, &SearchCompletionWindow::onResultListEntered);
    connect(result_view_, &DListView::pressed,
            this, &SearchCompletionWindow::onResultListClicked);
    connect(search_button_, &SearchButton::entered,
            this, &SearchCompletionWindow::onSearchButtonEntered);
    connect(search_button_, &SearchButton::pressed,
            this, &SearchCompletionWindow::searchButtonClicked);
}

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
    search_button_->setFixedHeight(35+7);
    search_button_->setText(QObject::tr("Search for \"%1\" in the full text"));
    search_button_->setGeometry(0, 7 * kItemHeight + 7, result_view_->width(), 34+7);

    this->setContentsMargins(0, 0, 0, 0);
    this->setMinimumHeight(kItemHeight);
    this->setFixedWidth(350);
    this->setWindowFlags(Qt::FramelessWindowHint |
                         Qt::CustomizeWindowHint |
                         Qt::BypassWindowManagerHint);
    this->setAttribute(Qt::WA_NativeWindow, true);
}

void SearchCompletionWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QStylePainter painter(this);
    QPainterPath path;
    path.addRect(this->rect());

    DPalette pa = ExApplicationHelper::instance()->palette(this);
    DStyleHelper styleHelper;
    QColor fillColor = pa.color(DPalette::FrameBorder);
    painter.fillPath(path, QBrush(fillColor));
}

void SearchCompletionWindow::onResultListClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        const int row = index.row();
        emit this->resultClicked(result_.at(row));
    } else {
        // Simulate button click event.
        emit this->searchButtonClicked();
    }

    this->hide();
}

void SearchCompletionWindow::onSearchButtonEntered()
{
    search_button_->setChecked(true);
    result_view_->setCurrentIndex(QModelIndex());
}

void SearchCompletionWindow::onResultListEntered(const QModelIndex &index)
{
    result_view_->setCurrentIndex(index);
    search_button_->setChecked(false);
}

}  // namespace dman
