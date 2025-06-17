// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "view/widget/search_button.h"
#include "base/utils.h"
#include "resources/themes/images.h"
#include "base/ddlog.h"

#include <DFontSizeManager>
#include <DPaletteHelper>

#include <QHBoxLayout>
#include <QStylePainter>
#include <QPainterPath>

DWIDGET_USE_NAMESPACE

SearchButton::SearchButton(QWidget *parent)
    : DBlurEffectWidget(parent)
    , m_bHover(false)
{
    qCDebug(app) << "SearchButton constructor called";
    setAutoFillBackground(false);

    m_textLabel = new DLabel(this);
    m_textLabel->setBackgroundRole(QPalette::Window);
    m_textLabel->setAutoFillBackground(false);
    m_textLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    iconBtn = new DIconButton(this);
    iconBtn->setIcon(DStyle::SP_IndicatorSearch);
    iconBtn->setFlat(true);
    iconBtn->setFocusPolicy(Qt::NoFocus);
    iconBtn->setAttribute(Qt::WA_TransparentForMouseEvents);

    iconBtn->setIconSize(QSize(20, 20));

    if (DGuiApplicationHelper::DarkType == DGuiApplicationHelper::instance()->themeType()) {
        qCDebug(app) << "Dark theme, use dark search icon";
        QPixmap iconPm = Utils::renderSVG(QString(kImageDarkSearchIcon), QSize(20, 20));
        iconBtn->setIcon(iconPm);
    } else {
        qCDebug(app) << "Light theme, use light search icon";
        QPixmap iconPm = Utils::renderSVG(QString(kImageLightSearchIcon), QSize(20, 20));
        iconBtn->setIcon(iconPm);
    }

    DPalette paLabel = DPaletteHelper::instance()->palette(m_textLabel);
    paLabel.setColor(DPalette::WindowText, paLabel.color(DPalette::WindowText));
    m_textLabel->setPalette(paLabel);

    DFontSizeManager::instance()->bind(m_textLabel, DFontSizeManager::T6, QFont::Medium);

    QHBoxLayout *centerLayout = new QHBoxLayout(this);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->setSpacing(0);

    centerLayout->addSpacing(30);
    centerLayout->addWidget(iconBtn, 0, Qt::AlignVCenter);
    centerLayout->addSpacing(8);
    centerLayout->addWidget(m_textLabel, 0, Qt::AlignCenter);
    centerLayout->addStretch();
    setLayout(centerLayout);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &SearchButton::onThemeChange);
    qCDebug(app) << "SearchButton constructor finished";
}

SearchButton::~SearchButton()
{
    qCDebug(app) << "SearchButton destructor called";
}

/**
 * @brief SearchButton::updateColor
 * @param color 系统活动色颜色
 * 获取系统活动色
 */
void SearchButton::updateColor(const QColor &color)
{
    qCDebug(app) << "SearchButton::updateColor() called with color:" << color;
    myColor = color;
}

/**
 * @brief SearchButton::setText
 * @param title 搜索关键字
 * 设置search_button中的文本为搜索关键字
 */
void SearchButton::setText(QString title)
{
    qCDebug(app) << "SearchButton::setText() called with title:" << title;
    m_textLabel->setText(title);
}

/**
 * @brief SearchButton::isChecked
 * @return
 * 判断鼠标是否在按钮上
 */
bool SearchButton::isChecked()
{
    qCDebug(app) << "is checked called: " << m_bHover;
    return m_bHover;
}

/**
 * @brief SearchButton::leaveFocus
 * 焦点移出时，改变searchIcon
 * 根据不同系统主题使用不同的图片
 */
void SearchButton::leaveFocus()
{
    qCDebug(app) << "leave focus called";
    m_bHover = false;
    if (DGuiApplicationHelper::DarkType == DGuiApplicationHelper::instance()->themeType()) {
        qCDebug(app) << "leave focus, update ui, dark theme";
        QPixmap iconPm = Utils::renderSVG(QString(kImageDarkSearchIcon), QSize(20, 20));
        iconBtn->setIcon(iconPm);
    } else {
        qCDebug(app) << "leave focus, update ui, light theme";
        QPixmap iconPm = Utils::renderSVG(QString(kImageLightSearchIcon), QSize(20, 20));
        iconBtn->setIcon(iconPm);
    }

    qCDebug(app) << "leave focus, update ui";
    update();
}

/**
 * @brief SearchButton::setChecked
 * @param bChecked 按钮状态
 * 修改按钮状态
 */
void SearchButton::setChecked(bool bChecked)
{
    qCDebug(app) << "set checked called: " << bChecked;
    m_bHover = bChecked;
    qCDebug(app) << "Search button hover state changed to:" << m_bHover;
    update();
}

/**
 * @brief SearchButton::onThemeChange
 * @param themeType 根据系统主题
 * 根据不同主题设置不同的searchIcon图片
 */
void SearchButton::onThemeChange(DGuiApplicationHelper::ColorType themeType)
{
    qCDebug(app) << "SearchButton theme changed to:" << (themeType == DGuiApplicationHelper::DarkType ? "Dark" : "Light");
    if (DGuiApplicationHelper::DarkType == themeType) {
        QPixmap iconPm = Utils::renderSVG(QString(kImageDarkSearchIcon), QSize(20, 20));
        iconBtn->setIcon(iconPm);
        qCDebug(app) << "Search button icon changed to dark";
    } else {
        QPixmap iconPm = Utils::renderSVG(QString(kImageLightSearchIcon), QSize(20, 20));
        iconBtn->setIcon(iconPm);
        qCDebug(app) << "Search button icon changed to light";
    }
    qCDebug(app) << "Search button theme changed, update ui";
}

/**
 * @brief SearchButton::paintEvent
 * @param event
 * 重写绘制事件
 */
void SearchButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QStylePainter painter(this);
    QPainterPath path;
    path.addRect(this->rect());

    if (m_bHover) {
        qCDebug(app) << "Search button hovered";
        DPalette pa = ExApplicationHelper::instance()->palette(this);
        DStyleHelper styleHelper;
        QColor fillColor = DGuiApplicationHelper::instance()->applicationPalette().highlight().color();
        painter.fillPath(path, QBrush(fillColor));

        DPalette paLabel = ExApplicationHelper::instance()->palette(this);
        paLabel.setColor(DPalette::WindowText, paLabel.color(DPalette::HighlightedText));
        m_textLabel->setPalette(paLabel);

        QPixmap iconPm = Utils::renderSVG(QString(kImageWhiteSearchIcon), QSize(20, 20));
        iconBtn->setIcon(iconPm);
    } else {
        qCDebug(app) << "Search button not hovered";
        DPalette pa = ExApplicationHelper::instance()->palette(this);
        DStyleHelper styleHelper;
        QColor fillColor = pa.color(DPalette::ItemBackground);
        painter.fillPath(path, QBrush(fillColor));

        DPalette paLabel = ExApplicationHelper::instance()->palette(this);
        paLabel.setColor(DPalette::WindowText, paLabel.color(DPalette::Text));
        m_textLabel->setPalette(paLabel);
        leaveFocus();
    }
}

void SearchButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    qCDebug(app) << "Search button pressed";
}

/**
 * @brief SearchButton::mouseReleaseEvent
 * @param event
 * 鼠标释放时发送信号，主窗口通过关键字进行全文搜索
 */
void SearchButton::mouseReleaseEvent(QMouseEvent *event)
{
    qCDebug(app) << "Search button clicked";
    QWidget::mouseReleaseEvent(event);
    emit pressed();
    qCDebug(app) << "Emitted pressed signal";
}

/**
 * @brief SearchButton::enterEvent
 * @param event
 * 鼠标移入边界时发送entered()信号,修改ListView索引，实现互斥
 */
void SearchButton::enterEvent(EnterEvent *event)
{
    Q_UNUSED(event);
    qCDebug(app) << "Mouse entered search button area";

    emit entered();
    m_bHover = true;
    update();
    qCDebug(app) << "Search button hover state changed to true";
}

/**
 * @brief SearchButton::leaveEvent
 * @param event
 * 鼠标移出边界时，根据不同系统主题色设置searchIcon图片
 */
void SearchButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    qCDebug(app) << "Mouse left search button area";

    m_bHover = false;
    //根据不同系统主题使用不同的Ｉｃｏｎ
    if (DGuiApplicationHelper::DarkType == DGuiApplicationHelper::instance()->themeType()) {
        qCDebug(app) << "Dark theme, change search button icon";
        QPixmap iconPm = Utils::renderSVG(QString(kImageDarkSearchIcon), QSize(20, 20));
        iconBtn->setIcon(iconPm);
    } else {
        qCDebug(app) << "Light theme, change search button icon";
        QPixmap iconPm = Utils::renderSVG(QString(kImageLightSearchIcon), QSize(20, 20));
        iconBtn->setIcon(iconPm);
    }

    update();
    qCDebug(app) << "Search button hover state changed to false";
}
