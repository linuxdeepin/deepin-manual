#ifndef SEARCH_COMPLETION_DELEGATE_H
#define SEARCH_COMPLETION_DELEGATE_H

#include <DStyledItemDelegate>

DWIDGET_USE_NAMESPACE

struct SearchCompletionItemModel
{
    QString strSearchAnchorId;
    QString strSearchKeyword;
    QString strSearchAppName;
    QString strSearchAppDisplayName;

    SearchCompletionItemModel()
    {
        strSearchAnchorId = "";
        strSearchKeyword = "";
        strSearchAppName = "";
        strSearchAppDisplayName = "";
    }
};

Q_DECLARE_METATYPE(SearchCompletionItemModel)

class SearchCompletionDelegate : public DStyledItemDelegate
{
    Q_OBJECT
public:
    SearchCompletionDelegate(QAbstractItemView *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

private:
    QAbstractItemView *m_parentView;
};

#endif // SEARCH_COMPLETION_DELEGATE_H
