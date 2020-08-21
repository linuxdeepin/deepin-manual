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

#include "view/widget/search_edit.h"

#include <QKeyEvent>
#include <QToolButton>
#include <QAction>
#include <QDebug>

#include "base/utils.h"

namespace dman {

SearchEdit::SearchEdit(QWidget *parent)
    : DSearchEdit(parent)
{
    //设置：当lineEidt中的closeButton被点击时发送信号onClickedClearBtn()
    QAction *clearAction = this->lineEdit()->findChild<QAction *>(QLatin1String("_q_qlineeditclearaction"));
    if (clearAction != nullptr) {
        QList<QToolButton *> list = this->lineEdit()->findChildren<QToolButton *>();
        for (int i = 0; i < list.count(); i++) {
            if (list.at(i)->defaultAction() == clearAction) {
                QToolButton *clearBtn = list.at(i);
                connect(clearBtn, &QToolButton::clicked, this, [ = ] {
                    emit this->onClickedClearBtn();
                    qDebug() << "emit onClickedClearBtn";
                });
            }
        }
    }
}

SearchEdit::~SearchEdit()
{
}

QString SearchEdit::getStr()
{
    return "abc";
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
        emit this->upKeyPressed();
        break;
    }
    case Qt::Key_Down: {
        emit this->downKeyPressed();
        break;
    }
    case Qt::Key_Return:
    case Qt::Key_Enter: {
        emit this->enterPressed();
        break;
    }
    default: {
    }
    }

    DSearchEdit::keyPressEvent(event);
}

} // namespace dman
