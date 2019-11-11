#include "search_completion_listview.h"

#include <QMouseEvent>

SearchCompletionListView::SearchCompletionListView(QWidget *parent)
    : DListView(parent)
{
    setBackgroundRole(QPalette::Background);
    setAutoFillBackground(false);

    initDelegate();
}

SearchCompletionListView::~SearchCompletionListView()
{
}

void SearchCompletionListView::initDelegate()
{
    m_searchCompletionDelegate = new SearchCompletionDelegate(this);
    this->setItemDelegate(m_searchCompletionDelegate);
}

QStandardItemModel* SearchCompletionListView::getErrorListSourceModel()
{
    return m_searchCompletionSourceModel;
}

void SearchCompletionListView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_bLeftMouse = true;
    } else {
        m_bLeftMouse = false;
    }

    DListView::mousePressEvent(event);
}

void SearchCompletionListView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command)
{
    DListView::setSelection(rect, command);

    QPoint selectionPoint(rect.x(), rect.y());
//    QModelIndex modelIndex = indexAt(selectionPoint);

    if (m_bLeftMouse) {

//        DFInstallErrorItemModel itemModel =
//            qvariant_cast<DFInstallErrorItemModel>(m_errorListSourceModel->data(modelIndex));
//        if (!itemModel.bSelectable) {
//            return;
//        }

//        if (selectionPoint.x() < FTM_ERROR_ITEM_FONTNAME_LEFT) {

//            emit onClickErrorListItem(modelIndex);
//        }
    }
}
