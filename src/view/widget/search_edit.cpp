// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "view/widget/search_edit.h"
#include "base/ddlog.h"

#include <QKeyEvent>
#include <QToolButton>
#include <QAction>
#include <QDebug>

SearchEdit::SearchEdit(QWidget *parent)
    : DSearchEdit(parent)
{
    qCDebug(app) << "SearchEdit constructor called";
    //设置：当lineEidt中的closeButton被点击时发送信号onClickedClearBtn()
    QAction *clearAction = this->lineEdit()->findChild<QAction *>(QLatin1String("_q_qlineeditclearaction"));
    if (clearAction != nullptr) {
        QList<QToolButton *> list = this->lineEdit()->findChildren<QToolButton *>();
        for (int i = 0; i < list.count(); i++) {
            if (list.at(i)->defaultAction() == clearAction) {
                QToolButton *clearBtn = list.at(i);
                connect(clearBtn, &QToolButton::clicked, this, [ = ] {
                    qCDebug(app) << "Search clear button clicked";
                    emit this->onClickedClearBtn();
                });
            }
        }
    }
}

SearchEdit::~SearchEdit()
{
    qCDebug(app) << "SearchEdit destructor called";
}

/**
 * @brief SearchEdit::keyPressEvent
 * @param event
 * 重写键盘事件，定义快捷键
 */
void SearchEdit::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up: {
        qCDebug(app) << "Up arrow key pressed in search edit";
        emit this->upKeyPressed();
        break;
    }
    case Qt::Key_Down: {
        qCDebug(app) << "Down arrow key pressed in search edit";
        emit this->downKeyPressed();
        break;
    }
    case Qt::Key_Return:
        qCDebug(app) << "Enter/Return key pressed in search edit";
    case Qt::Key_Enter: {
        emit this->enterPressed();
        break;
    }
    default: {
    }
    }

    DSearchEdit::keyPressEvent(event);
}
