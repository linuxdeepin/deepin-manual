// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "search_completion_listview.h"
#include "base/utils.h"
#include "base/ddlog.h"

#include <QMouseEvent>

SearchCompletionListView::SearchCompletionListView(QWidget *parent)
    : DListView(parent)
{
    qCDebug(app) << "SearchCompletionListView constructor called";
    setAutoFillBackground(false);
    initDelegate();
    qCDebug(app) << "SearchCompletionListView initialization completed";
}

SearchCompletionListView::~SearchCompletionListView()
{
    qCDebug(app) << "SearchCompletionListView destructor called";
}

/**
 * @brief SearchCompletionListView::initDelegate
 * 初始化代理
 */
void SearchCompletionListView::initDelegate()
{
    qCDebug(app) << "Initializing search completion delegate";
    m_searchCompletionDelegate = new SearchCompletionDelegate(this);
    this->setItemDelegate(m_searchCompletionDelegate);
    qCDebug(app) << "Search completion delegate initialized";
}

/**
 * @brief SearchCompletionListView::mousePressEvent
 * @param event
 * 重写鼠标点击事件，判断是否为左键点击
 */
void SearchCompletionListView::mousePressEvent(QMouseEvent *event)
{
    qCDebug(app) << "Mouse press event received at position:" << event->pos();

    if (event->button() == Qt::LeftButton) {
        m_bLeftMouse = true;
        qCDebug(app) << "Left mouse button pressed";
    } else {
        m_bLeftMouse = false;
        qCDebug(app) << "Non-left mouse button pressed";
    }

    DListView::mousePressEvent(event);
    qCDebug(app) << "Mouse press event processing completed";
}

/**
 * @brief SearchCompletionListView::setSelection
 * @param rect
 * @param command
 * 视图选择，鼠标选中listview中的的item时，根据坐标获取mmodeIndex
 * 如果是左键点击，把索引通过onClickSearchCompletionItem()信号发送给search_completion_window
 */
void SearchCompletionListView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command)
{
    qCDebug(app) << "SearchCompletionListView selecting item at position:" << rect.topLeft();
    DListView::setSelection(rect, command);

    QPoint clickPoint(rect.x(), rect.y());
    QModelIndex modelIndex = indexAt(clickPoint);
    qCDebug(app) << "Selected model index:" << modelIndex << "at position:" << clickPoint;

    if (m_bLeftMouse) {
        qCDebug(app) << "Left mouse click on search completion item:" << modelIndex;
        qCDebug(app) << "Emitting onClickSearchCompletionItem signal";
        emit onClickSearchCompletionItem(modelIndex);
    }
}
