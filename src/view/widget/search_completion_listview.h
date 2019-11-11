#ifndef SEARCH_COMPLETION_LISTVIEW_H
#define SEARCH_COMPLETION_LISTVIEW_H

#include "search_completion_delegate.h"

#include <QMetaType>
#include <QString>

#include <DCheckBox>
#include <DLabel>
#include <DListView>

DWIDGET_USE_NAMESPACE

class SearchCompletionListView : public DListView
{
    Q_OBJECT

public:
    explicit SearchCompletionListView(QWidget *parent = nullptr);
    ~SearchCompletionListView() override;

    void mousePressEvent(QMouseEvent *event) override;
    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command) override;
    void initDelegate();

    QStandardItemModel* getErrorListSourceModel();

private:

    SearchCompletionDelegate *m_searchCompletionDelegate;
    QStandardItemModel *m_searchCompletionSourceModel;
    QList<SearchCompletionItemModel> m_searchCompletionModelList;

    bool m_bLeftMouse;

signals:
    void onClickSearchCompletionItem(QModelIndex index);

private slots:

};

#endif // SEARCH_COMPLETION_LISTVIEW_H
