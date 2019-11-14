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

#include "view/widget/title_bar.h"

#include <QDebug>
#include <QTimer>
#include <DIconButton>

#include "view/theme_manager.h"
#include "view/widget/search_edit.h"

DWIDGET_USE_NAMESPACE

namespace dman {

TitleBar::TitleBar(QWidget *parent) : QFrame(parent)
{
    this->setObjectName("TitleBar");
    this->initUI();
    this->initConnections();
}

TitleBar::~TitleBar()
{
}

QString TitleBar::getSearchText() const
{
    QString text = search_edit_->text();
    return text.remove('\n').remove('\r').remove("\r\n");
}

void TitleBar::setBackwardButtonActive(bool active)
{
    back_button_->setEnabled(active);
}

void TitleBar::setForwardButtonActive(bool active)
{
    forward_button_->setEnabled(active);
}

void TitleBar::initConnections()
{
    connect(back_button_, &DIconButton::clicked,
            this, &TitleBar::backwardButtonClicked);
    connect(forward_button_, &DIconButton::clicked,
            this, &TitleBar::forwardButtonClicked);
    connect(search_edit_, &SearchEdit::textChanged,
            this, &TitleBar::onSearchTextChanged);
    connect(search_edit_, &SearchEdit::focusChanged,
            this, &TitleBar::focusOut);
    connect(search_edit_, &SearchEdit::downKeyPressed,
            this, &TitleBar::downKeyPressed);
    connect(search_edit_, &SearchEdit::enterPressed,
            this, &TitleBar::enterPressed);
    connect(search_edit_, &SearchEdit::upKeyPressed,
            this, &TitleBar::upKeyPressed);
}

void TitleBar::initUI()
{
//    QHBoxLayout *buttonLayout = new QHBoxLayout;
//    buttonLayout->setMargin(0);
//    buttonLayout->setSpacing(0);
    forward_button_ = new DIconButton(DStyle::SP_ArrowNext);
    forward_button_->setFlat(false);
    back_button_ = new DIconButton(DStyle::SP_ArrowPrev);
    back_button_->setFlat(false);
//    buttonLayout->addWidget(forward_button_);
//    buttonLayout->addWidget(back_button_);
//    buttonLayout->addStretch();
//    QFrame *buttonFrame = new QFrame(this);
//    buttonFrame->setLayout(buttonLayout);
//    buttonFrame->setFixedSize(250, 50);


    search_edit_ = new SearchEdit(this);
    search_edit_->setObjectName("SearchEdit");
    search_edit_->setFixedSize(350, 44);
    search_edit_->setPlaceHolder(QObject::tr("Search"));

    QHBoxLayout *main_layout = new QHBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);

    main_layout->addStretch();
    main_layout->addWidget(search_edit_);
    this->setLayout(main_layout);

    this->setAttribute(Qt::WA_TranslucentBackground, true);
}


void TitleBar::onSearchTextChanged()
{
    emit this->searchTextChanged(search_edit_->text());
}

}  // namespace dman
