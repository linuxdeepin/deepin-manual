// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "view/widget/search_completion_window.h"
#include "view/widget/search_button.h"
#include "base/utils.h"
#include "base/ddlog.h"

#include <DWindowManagerHelper>
#include <DPaletteHelper>
#include <DGuiApplicationHelper>

#include <QStylePainter>
#include <QPainterPath>

namespace {

const int kItemHeight = 34;

} // namespace

SearchCompletionWindow::SearchCompletionWindow(QWidget *parent)
    : DBlurEffectWidget(parent)
{
    qCDebug(app) << "SearchCompletionWindow constructor called";
    this->setObjectName("SearchCompletionWindow");

    this->initUI();
    this->initConnections();
    qCDebug(app) << "SearchCompletionWindow initialization completed";
}

SearchCompletionWindow::~SearchCompletionWindow()
{
    qCDebug(app) << "SearchCompletionWindow destructor called";
}

/**
 * @brief SearchCompletionWindow::updateColor
 * @param color 系统活动色
 * 修改系统活动色，使searchButon背景色与系统活动色一致
 */
void SearchCompletionWindow::updateColor(const QColor &color)
{
    qCDebug(app) << "Updating button color";
    if (search_button_) {
        search_button_->updateColor(color);
        qCDebug(app) << "Button color updated";
    }
}

/**
 * @brief SearchCompletionWindow::autoResize
 * 根据搜索结果的数量自动设置窗口高度，最多显示7行结果，支持滚屏
 * 设置searchButton位置在listview下方
 */
void SearchCompletionWindow::autoResize()
{
    qCDebug(app) << "Auto resizing window based on result count";
    int rowCount = result_view_->model()->rowCount();

    int resultViewHeight = kItemHeight;
    if (rowCount > 0 && rowCount < 7) {
        qCDebug(app) << "Result count 0 ~ 7";
        resultViewHeight = rowCount * kItemHeight + 7;
        result_view_->setFixedHeight(resultViewHeight);
        this->setFixedHeight(resultViewHeight + kItemHeight + 7);
        search_button_->setGeometry(0, resultViewHeight, result_view_->width(), kItemHeight + 7);
    } else {
        qCDebug(app) << "Result count > 7";
        resultViewHeight = 7 * kItemHeight + 7;
        result_view_->setFixedHeight(7 * kItemHeight + 7);
        this->setFixedHeight(resultViewHeight + kItemHeight + 7);
        search_button_->setGeometry(0, resultViewHeight, result_view_->width(), kItemHeight + 7);
    }

    result_view_->setFixedWidth(this->width());
    search_button_->setFixedWidth(this->width());
    result_view_->setVisible(search_compeletion_model_->rowCount() > 0);
    qCDebug(app) << "Window auto resized";
}

/**
 * @brief SearchCompletionWindow::goDown
 * 方向键“下”，实现可循环滚动效果
 */
void SearchCompletionWindow::goDown()
{
    qCDebug(app) << "Processing down arrow key navigation";
    if (nullptr == search_compeletion_model_) {
        qCDebug(app) << "Search completion model is null";
        return;
    }

    if (search_compeletion_model_->rowCount() == 0) {
        qCDebug(app) << "Result count 0";
        search_button_->setChecked(true);
    } else {
        if (search_button_->isChecked()) {
            qCDebug(app) << "Search button is checked";
            search_button_->setChecked(false);
            const QModelIndex first_idx = search_compeletion_model_->index(0, 0);
            result_view_->setCurrentIndex(first_idx);
        } else {
            qCDebug(app) << "Search button is not checked";
            const int down_row = result_view_->currentIndex().row() + 1;
            if (down_row >= search_compeletion_model_->rowCount()) {
                qCDebug(app) << "Down row >= row count";
                search_button_->setChecked(true);
                result_view_->setCurrentIndex(QModelIndex());
            } else {
                qCDebug(app) << "Moving down to row:" << down_row;
                const QModelIndex down_idx = search_compeletion_model_->index(down_row, 0);
                result_view_->setCurrentIndex(down_idx);
            }
        }
    }
    qCDebug(app) << "Down arrow key navigation processed";
}

/**
 * @brief SearchCompletionWindow::goUp
 * 方向键“上”，实现可循环滚动效果
 */
void SearchCompletionWindow::goUp()
{
    qCDebug(app) << "Processing up arrow key navigation";
    if (nullptr == search_compeletion_model_) {
        qCDebug(app) << "Search completion model is null";
        return;
    }
    if (search_compeletion_model_->rowCount() == 0) {
        qCDebug(app) << "Result count 0";
        search_button_->setChecked(true);
    } else {
        if (search_button_->isChecked()) {
            qCDebug(app) << "Search button is checked";
            search_button_->setChecked(false);
            // Select last item.
            const QModelIndex idx = search_compeletion_model_->index(search_compeletion_model_->rowCount() - 1, 0);
            result_view_->setCurrentIndex(idx);
        } else {
            qCDebug(app) << "Search button is not checked";
            const int up_row = result_view_->currentIndex().row() - 1;
            if (up_row < 0) {
                qCDebug(app) << "Up row < 0";
                result_view_->setCurrentIndex(QModelIndex());
                search_button_->setChecked(true);
            } else {
                qCDebug(app) << "Moving up to row:" << up_row;
                const QModelIndex up_idx = search_compeletion_model_->index(up_row, 0);
                result_view_->setCurrentIndex(up_idx);
            }
        }
    }
    qCDebug(app) << "Up arrow key navigation processed";
}

/**
 * @brief SearchCompletionWindow::onEnterPressed
 * 焦点在searchEdit输入关键字后 点击回车键触发该槽
 * 如果searchButton为选择状态时执行全文搜索
 * 否则通过listViewItem的索引获取选中行的内容，执行页面显示
 */
void SearchCompletionWindow::onEnterPressed()
{
    qCDebug(app) << "Enter key pressed, current selection:"
             << (search_button_->isChecked() ? "search button" : "result item");
    if (search_button_->isChecked()) {
        qCDebug(app) << "Search button is checked, performing full text search";
        //最后选项在内容中查找结果 WebWindow::onSearchButtonClicked
        emit this->searchButtonClicked();
    } else {
        qCDebug(app) << "Result item is selected, getting content and emitting resultClicked signal";
        const QModelIndex idx = result_view_->currentIndex();
        //搜索结果选项单击
        this->onResultListClicked(idx);
    }
    // 点击任何一个Ｉｔｅｍ后都隐藏整个窗口
    this->hide();
    qCDebug(app) << "Enter key processed";
}

/**
 * @brief SearchCompletionWindow::setKeyword
 * @param keyword 搜索关键字
 * 设置seearchButton中的搜索关键字
 */
void SearchCompletionWindow::setKeyword(const QString &keyword)
{
    qCDebug(app) << "Setting search keyword:" << keyword;
    keyword_ = keyword;
    QFontMetrics metrics = search_button_->fontMetrics();
    search_button_->setText(
                metrics.elidedText(
                    QObject::tr("Search for \"%1\" in the full text").arg(keyword),
                    Qt::ElideRight, 350 - 39));
    qCDebug(app) << "Search keyword set";
}

/**
 * @brief SearchCompletionWindow::setSearchAnchorResult
 * @param result 搜索结果内容
 * 数据转换，全文搜索结果数据添加到searchDataList中，调用initSearchCompletionListData()方法，设置ListView;
 */
void SearchCompletionWindow::setSearchAnchorResult(const SearchAnchorResultList &result)
{
    qCDebug(app) << "Setting search results, count:" << result.size();
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
    //查询结果显示
    initSearchCompletionListData(searchDataList);
    this->autoResize();
    qCDebug(app) << "Search results set";
}

/**
 * @brief SearchCompletionWindow::initConnections
 * 信号槽连接
 */
void SearchCompletionWindow::initConnections()
{
    qCDebug(app) << "Connecting result view signals";
    connect(result_view_, &SearchCompletionListView::onClickSearchCompletionItem,
            this, &SearchCompletionWindow::onResultListClicked);
    connect(result_view_, &DListView::entered,
            this, &SearchCompletionWindow::onResultListEntered);
    connect(search_button_, &SearchButton::entered,
            this, &SearchCompletionWindow::onSearchButtonEntered);
    connect(search_button_, &SearchButton::pressed,
            this, &SearchCompletionWindow::searchButtonClicked);
    qCDebug(app) << "Result view signals connected";
}

/**
 * @brief SearchCompletionWindow::initSearchCompletionListData
 * @param dataList 搜索结果list
 * 全文搜索结果数据，添加到QStandardItemModel中，通过setModel()，设置ListView
 */
void SearchCompletionWindow::initSearchCompletionListData(QList<SearchCompletionItemModel> dataList)
{
    qCDebug(app) << "Initializing search completion list data, count:" << dataList.size();
    search_compeletion_model_ = new QStandardItemModel(result_view_);

    for (int i = 0; i < dataList.size(); i++) {
        QStandardItem *item = new QStandardItem;
        SearchCompletionItemModel itemModel = dataList.at(i);
        item->setData(QVariant::fromValue(itemModel), Qt::DisplayRole);

        search_compeletion_model_->appendRow(item);
    }

    result_view_->setModel(search_compeletion_model_);
    qCDebug(app) << "Search completion list data initialized";
}

/**
 * @brief SearchCompletionWindow::initUI
 * 界面初始化，创建ListView对象
 */
void SearchCompletionWindow::initUI()
{
    qCDebug(app) << "Initializing search completion window UI";
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
    if(!Utils::judgeWayLand()){
        qCDebug(app) << "Setting window flags";
        this->setWindowFlags(Qt::FramelessWindowHint
                             | Qt::CustomizeWindowHint
                             | Qt::BypassWindowManagerHint);
        this->setAttribute(Qt::WA_NativeWindow, true);
    }
    qCDebug(app) << "Search completion window UI initialized";
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

    if(!Utils::judgeWayLand()){
        if (DWindowManagerHelper::instance()->hasComposite()) {
            fillColor = pa.color(DPalette::FrameBorder);
            qCDebug(app) << "Using frame border color:" << fillColor;
        } else {
            DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
            if (themeType == DGuiApplicationHelper::LightType) {
                fillColor = QColor(255, 255, 255);
                qCDebug(app) << "Using light theme color";
            } else if (themeType == DGuiApplicationHelper::DarkType) {
                fillColor = QColor(0, 0, 0);
                qCDebug(app) << "Using dark theme color";
            }
        }
    }else {
        DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
        if (themeType == DGuiApplicationHelper::LightType) {
            fillColor = QColor(255, 255, 255);
            setMaskAlpha(255);
            qCDebug(app) << "Using light theme color for Wayland";
        } else if (themeType == DGuiApplicationHelper::DarkType) {
            fillColor = QColor(0, 0, 0);
            setMaskAlpha(255);
            qCDebug(app) << "Using dark theme color for Wayland";
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
    qCDebug(app) << "Result list item clicked, row:" << index.row();
    if (index.isValid()) {
        qCDebug(app) << "Valid index, getting content and emitting resultClicked signal";
        const int row = index.row();
        //发送信号->WebWindow::onSearchResultClicked
        emit this->resultClicked(result_.at(row));
        result_view_->setCurrentIndex(QModelIndex());
    } else {
        qCDebug(app) << "Invalid index, simulating search button click";
        // 模拟按钮单击事件。
        emit this->searchButtonClicked();
    }
    this->hide();
    qCDebug(app) << "Result list item clicked processed";
}

/**
 * @brief SearchCompletionWindow::onSearchButtonEntered
 * searchButton被选中，修改状态，并设置ListView索引
 */
void SearchCompletionWindow::onSearchButtonEntered()
{
    qCDebug(app) << "Search button entered, setting checked state";
    search_button_->setChecked(true);
    result_view_->setCurrentIndex(QModelIndex());
    qCDebug(app) << "Search button entered processed";
}

/**
 * @brief SearchCompletionWindow::onResultListEntered
 * @param index
 * 绑定DListView::entered()信号，鼠标移入Item时解发
 */
void SearchCompletionWindow::onResultListEntered(const QModelIndex &index)
{
    qCDebug(app) << "Result list entered, setting current index to row:" << index.row();
    result_view_->setCurrentIndex(index);
    search_button_->setChecked(false);
    qCDebug(app) << "Result list entered processed";
}
