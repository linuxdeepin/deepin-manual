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

#include "view/widget/search_button.h"
#include "base/utils.h"
#include "resources/themes/images.h"

#include <QHBoxLayout>
#include <QStylePainter>
#include <QMouseEvent>

#include <DStyleHelper>
#include <DApplicationHelper>
#include <DFontSizeManager>

#include <DLog>

DWIDGET_USE_NAMESPACE

namespace dman {

SearchButton::SearchButton(QWidget *parent)
    : DBlurEffectWidget(parent)
    , m_bHover(false)
{
    setAutoFillBackground(false);

    m_textLabel = new DLabel(this);
    m_textLabel->setBackgroundRole(QPalette::Background);
    m_textLabel->setAutoFillBackground(false);
    m_textLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    iconBtn = new DIconButton(this);
    iconBtn->setIcon(DStyle::SP_IndicatorSearch);
    iconBtn->setFlat(true);
    iconBtn->setFocusPolicy(Qt::NoFocus);
    iconBtn->setAttribute(Qt::WA_TransparentForMouseEvents);

    iconBtn->setIconSize(QSize(20, 20));

    if (DGuiApplicationHelper::DarkType == DGuiApplicationHelper::instance()->themeType()) {
        QPixmap iconPm = Utils::renderSVG(QString(kImageDarkSearchIcon), QSize(20, 20));
        iconBtn->setIcon(iconPm);
    } else {
        QPixmap iconPm = Utils::renderSVG(QString(kImageLightSearchIcon), QSize(20, 20));
        iconBtn->setIcon(iconPm);
    }

    DPalette paLabel = DApplicationHelper::instance()->palette(m_textLabel);
//    paLabel.setColor(DPalette::WindowText, paLabel.color(DPalette::WindowText));
    paLabel.setColor(DPalette::WindowText, paLabel.color(DPalette::WindowText));
    m_textLabel->setPalette(paLabel);

    DFontSizeManager::instance()->bind(m_textLabel, DFontSizeManager::T6, QFont::Medium);

    QHBoxLayout *centerLayout = new QHBoxLayout(this);
    centerLayout->setMargin(0);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->setSpacing(0);

    centerLayout->addSpacing(30);
    centerLayout->addWidget(iconBtn, 0, Qt::AlignVCenter);
    centerLayout->addSpacing(8);
    centerLayout->addWidget(m_textLabel, 0, Qt::AlignCenter);
    centerLayout->addStretch();
    setLayout(centerLayout);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &SearchButton::onThemeChange);
}

SearchButton::~SearchButton()
{

}

void SearchButton::updateColor(const QColor &color)
{
    myColor = color;
}

void SearchButton::setText(QString title)
{
    m_textLabel->setText(title);
}

bool SearchButton::isChecked()
{
    return m_bHover;
}

void SearchButton::setChecked(bool bChecked)
{
    m_bHover = bChecked;
    update();
}

void SearchButton::onThemeChange(DGuiApplicationHelper::ColorType themeType)
{
    if (DGuiApplicationHelper::DarkType == themeType) {
        QPixmap iconPm = Utils::renderSVG(QString(kImageDarkSearchIcon), QSize(20, 20));
        iconBtn->setIcon(iconPm);
    } else {
        QPixmap iconPm = Utils::renderSVG(QString(kImageLightSearchIcon), QSize(20, 20));
        iconBtn->setIcon(iconPm);
    }
}

void SearchButton::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event)

    QStylePainter painter(this);
    QPainterPath path;
    path.addRect(this->rect());

    if (m_bHover) {
        DPalette pa = ExApplicationHelper::instance()->palette(this);
        DStyleHelper styleHelper;
        painter.fillPath(path, QBrush(myColor));

        DPalette paLabel = ExApplicationHelper::instance()->palette(this);
        paLabel.setColor(DPalette::WindowText, paLabel.color(DPalette::HighlightedText));
        m_textLabel->setPalette(paLabel);
    } else {
        DPalette pa = ExApplicationHelper::instance()->palette(this);
        DStyleHelper styleHelper;
        QColor fillColor = pa.color(DPalette::DarkLively);
        painter.fillPath(path, QBrush(fillColor));
//        painter.fillPath(path, QBrush(myColor));

        DPalette paLabel = ExApplicationHelper::instance()->palette(this);
        paLabel.setColor(DPalette::WindowText, paLabel.color(DPalette::Text));
        m_textLabel->setPalette(paLabel);
    }
}

void SearchButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void SearchButton::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);

    emit pressed();
}

void SearchButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event);

    emit entered();
    m_bHover = true;

    QPixmap iconPm = Utils::renderSVG(QString(kImageDarkSearchIcon), QSize(20, 20));
    iconBtn->setIcon(iconPm);

    update();
}

void SearchButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);

    m_bHover = false;

//    QPixmap iconPm = Utils::renderSVG(QString(kImageLightSearchIcon), QSize(20, 20));
//    iconBtn->setIcon(iconPm);

    if (DGuiApplicationHelper::DarkType == DGuiApplicationHelper::instance()->themeType()) {

        QPixmap iconPm = Utils::renderSVG(QString(kImageDarkSearchIcon), QSize(20, 20));
        iconBtn->setIcon(iconPm);
    } else {

        QPixmap iconPm = Utils::renderSVG(QString(kImageLightSearchIcon), QSize(20, 20));
        iconBtn->setIcon(iconPm);
    }

    update();
}

}  // namespace dman
