#include "search_completion_listview.h"
#include "base/utils.h"

#include <QMouseEvent>

SearchCompletionListView::SearchCompletionListView(QWidget *parent)
    : DListView(parent)
{
    setAutoFillBackground(false);
    initDelegate();
}

SearchCompletionListView::~SearchCompletionListView()
{
}

/**
 * @brief SearchCompletionListView::initDelegate
 * 初始化代理
 */
void SearchCompletionListView::initDelegate()
{
    m_searchCompletionDelegate = new SearchCompletionDelegate(this);
    this->setItemDelegate(m_searchCompletionDelegate);
}

/**
 * @brief SearchCompletionListView::mousePressEvent
 * @param event
 * 重写鼠标点击事件，判断是否为左键点击
 */
void SearchCompletionListView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_bLeftMouse = true;
    } else {
        m_bLeftMouse = false;
    }

    DListView::mousePressEvent(event);
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
    DListView::setSelection(rect, command);

    QPoint clickPoint(rect.x(), rect.y());
    QModelIndex modelIndex = indexAt(clickPoint);

    if (m_bLeftMouse) {
        emit onClickSearchCompletionItem(modelIndex);
    }
}
