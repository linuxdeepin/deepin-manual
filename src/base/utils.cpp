/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
 * Maintainer: rekols <rekols@foxmail.com>
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

#include "utils.h"

#include <DLog>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFontDatabase>
#include <QFontInfo>
#include <QImageReader>
#include <QMimeDatabase>
#include <QMimeType>
#include <QPixmap>
#include <QStandardPaths>
#include <QTextCodec>
#include <QUrl>

QHash<QString, QPixmap> Utils::m_imgCacheHash;
QHash<QString, QString> Utils::m_fontNameCache;

//标题映射表
const int langCount = 3;
QString languageArr[][langCount] = {
    //dde model
    {"controlcenter", "控制中心", "Control Center"},
    {"accounts", "账户设置", "Account Settings"},
    {"cloudsync", "网络帐户", "Cloud Account"},
    {"display", "显示设置", "Display Settings"},
    {"defapp", "默认程序设置", "Default Application Settings"},
    {"personalization", "个性化设置", "Personalization Settings"},
    {"network", "网络设置", "Network Settings"},
    {"sound", "声音设置", "Sound Settings"},
    {"bluetooth", "蓝牙设置", "Sound Settings"},
    {"datetime", "时间日期", "Date and Time"},
    {"power", "电源管理", "Power Management"},
    {"mouse", "鼠标和触控板", "Mouse and Touchpad"},
    {"tablet", "数位板", "Drawing Tablet"},
    {"keyboard", "键盘和语言", "Keyboard and Language"},
    {"voice", "辅助功能", "Assistive Tools"},
    {"update", "系统更新", "Update Settings"},
    {"systeminfo", "系统信息", "System Info"},
    {"License activator", "授权管理", "Authorization Management"},
    {"commoninfo", "通用设置", "General Settings"}
};

Utils::Utils(QObject *parent)
    : QObject(parent)
{
}

Utils::~Utils() {}

struct timeval Utils::getTime()
{
    struct timeval tp;
    gettimeofday(&tp, nullptr);
    return tp;
}

struct timeval Utils::showDiffTime(struct timeval tpStart)
{
    struct timeval tpEnd;
    gettimeofday(&tpEnd, nullptr);
    double timeuse =
        (1000000 * (tpEnd.tv_sec - tpStart.tv_sec) + tpEnd.tv_usec - tpStart.tv_usec) / 1000000.0;
    qDebug() << __FUNCTION__ << __LINE__ << timeuse << endl;

    return tpEnd;
}

QString Utils::getQssContent(const QString &filePath)
{
    QFile file(filePath);
    QString qss;

    if (file.open(QIODevice::ReadOnly)) {
        qss = file.readAll();
    }

    return qss;
}

bool Utils::isFontMimeType(const QString &filePath)
{
    const QString mimeName = QMimeDatabase().mimeTypeForFile(filePath).name();
    ;

    if (mimeName.startsWith("font/") || mimeName.startsWith("application/x-font")) {
        return true;
    }

    return false;
}

QString Utils::suffixList()
{
    return QString("Font Files (*.ttf *.ttc *.otf)");
}

QPixmap Utils::renderSVG(const QString &filePath, const QSize &size)
{
    if (m_imgCacheHash.contains(filePath)) {
        return m_imgCacheHash.value(filePath);
    }

    QImageReader reader;
    QPixmap pixmap;

    reader.setFileName(filePath);

    if (reader.canRead()) {
        const qreal ratio = qApp->devicePixelRatio();
        reader.setScaledSize(size * ratio);
        pixmap = QPixmap::fromImage(reader.read());
        pixmap.setDevicePixelRatio(ratio);
    } else {
        pixmap.load(filePath);
    }

    m_imgCacheHash.insert(filePath, pixmap);

    return pixmap;
}

QString Utils::loadFontFamilyByType(FontType fontType)
{
    QString fontFileName = "";
    switch (fontType) {
    case SourceHanSansMedium:
        fontFileName = ":/font/SourceHanSansCN-Medium.ttf";
        break;
    case SourceHanSansNormal:
        fontFileName = ":/font/SourceHanSansCN-Normal.ttf";
        break;
    case DefautFont:
        QFont font;
        return font.family();
    }

    if (m_fontNameCache.contains(fontFileName)) {
        return m_fontNameCache.value(fontFileName);
    }

    QString fontFamilyName = "";
    QFile fontFile(fontFileName);
    if (!fontFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Open font file error";
        return fontFamilyName;
    }

    int loadedFontID = QFontDatabase::addApplicationFontFromData(fontFile.readAll());
    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(loadedFontID);
    if (!loadedFontFamilies.empty()) {
        fontFamilyName = loadedFontFamilies.at(0);
    }
    fontFile.close();

    m_fontNameCache.insert(fontFileName, fontFamilyName);
    return fontFamilyName;
}

QFont Utils::loadFontBySizeAndWeight(QString fontFamily, int fontSize, int fontWeight)
{
    QFont font(fontFamily);
    font.setPixelSize(fontSize);
    font.setWeight(fontWeight);

    return font;
}

QString Utils::fromSpecialEncoding(const QString &inputStr)
{
    qDebug() << "inputStr is:" << inputStr << endl;
    bool bFlag = inputStr.contains(QRegExp("[\\x4e00-\\x9fa5]+"));
    if (bFlag) {
        return inputStr;
    }

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    if (codec) {
        QString unicodeStr = codec->toUnicode(inputStr.toLatin1());
        qDebug() << "convert to unicode:" << unicodeStr << endl;
        return unicodeStr;
    } else {
        return inputStr;
    }
}

/**
 * @brief Utils::translateTitle 返回title映射字段，目前主要用于＂控制中心＂跳转
 * @param titleUS
 * @return
 * @note 除简体中文外,其他暂时都当英文.
 */
QString Utils::translateTitle(const QString &titleUS)
{
    QString strRet = titleUS;
    int nCount = sizeof(languageArr) / sizeof(languageArr[0]);
    for (int i = 0; i < nCount; i++) {
        if (languageArr[i][0] == titleUS) {
            if (QLocale::system().name() == "zh_CN") {
                strRet = languageArr[i][1];
            } else {
                strRet = languageArr[i][2];
            }
        }
    }
    return strRet;
}

ExApplicationHelper *ExApplicationHelper::instance()
{
    return qobject_cast<ExApplicationHelper *>(DGuiApplicationHelper::instance());
}

#define CAST_INT static_cast<int>

QColor light_qpalette[QPalette::NColorRoles] {
    QColor(0x00, 0x00, 0x00),                         // WindowText
    QColor(0xe5, 0xe5, 0xe5),                         // Button
    QColor(0xe6, 0xe6, 0xe6),                         // Light
    QColor(0xe5, 0xe5, 0xe5),                         // Midlight
    QColor(0xe3, 0xe3, 0xe3),                         // Dark
    QColor(0xe4, 0xe4, 0xe4),                         // Mid
    QColor(0x41, 0x4d, 0x68),                         // Text
    Qt::black,                                   // BrightText
    QColor(0x41, 0x4d, 0x68),                         // ButtonText
    Qt::white,                                   // Base
    QColor(0xf8, 0xf8, 0xf8),                         // Window
    QColor(0, 0, 0, CAST_INT(0.05 * 255)),       // Shadow
    QColor(0x00, 0x81, 0xff),                           // Highlight
    Qt::white,                                   // HighlightedText
    QColor(0x00, 0x82, 0xfa),                         // Link
    QColor(0xad, 0x45, 0x79),                         // LinkVisited
    QColor(0, 0, 0, CAST_INT(0.03 * 255)),       // AlternateBase
    Qt::white,                                   // NoRole
    QColor(255, 255, 255, CAST_INT(0.8 * 255)),  // ToolTipBase
    Qt::black                                    // ToolTipText
};

QColor dark_qpalette[QPalette::NColorRoles] {
    QColor(0xff, 0xff, 0xff),                      // WindowText
    QColor(0x44, 0x44, 0x44),                      // Button
    QColor(0x48, 0x48, 0x48),                      // Light
    QColor(0x47, 0x47, 0x47),                      // Midlight
    QColor(0x41, 0x41, 0x41),                      // Dark
    QColor(0x43, 0x43, 0x43),                      // Mid
    QColor(0xc0, 0xc6, 0xd4),                      // Text
    Qt::white,                                // BrightText
    QColor(0xc0, 0xc6, 0xd4),                      // ButtonText
    QColor(0x28, 0x28, 0x28),                      // Base
    QColor(0x25, 0x25, 0x25),                      // Window
    QColor(0, 0, 0, CAST_INT(0.05 * 255)),    // Shadow
    QColor(0x00, 0x81, 0xff),                      // Highlight
    QColor(0xF1, 0xF6, 0xFF),                      // HighlightedText
    QColor(0x00, 0x82, 0xfa),                      // Link
    QColor(0xad, 0x45, 0x79),                      // LinkVisited
    QColor(0, 0, 0, CAST_INT(0.05 * 255)),    // AlternateBase
    Qt::black,                                // NoRole
    QColor(45, 45, 45, CAST_INT(0.8 * 255)),  // ToolTipBase
    QColor(0xc0, 0xc6, 0xd4)                       // ToolTipText
};

QColor light_dpalette[DPalette::NColorTypes] {
    QColor(),                                    // NoType
    QColor(255, 255, 255, CAST_INT(0.2 * 255)),  // ItemBackground
//    QColor("#001A2E"),                           // TextTitle
//    QColor("#8AA1B4"),                           // TextTips
//    QColor("#FF5736"),                           // TextWarning
//    QColor("#0082FA"),                           // TextLively
//    QColor("#25b7ff"),                           // LightLively
    QColor(0x00, 0x1A, 0x2E),                         // TextTitle
    QColor(0x8A, 0xA1, 0xB4),                         // TextTips
    QColor(0xFF, 0x57, 0x36),                         // TextWarning
    QColor(0x00, 0x82, 0xFA),                         // TextLively
    QColor(0x25, 0xb7, 0xff),                         // LightLively

    QColor(235, 235, 235, CAST_INT(0 * 255)),    // DarkLively
    QColor(235, 235, 235, CAST_INT(0 * 255))     // FrameBorder
};

QColor dark_dpalette[DPalette::NColorTypes] {
    QColor(),                                // NoType
    QColor(25, 25, 25, CAST_INT(0 * 255)),   // ItemBackground
//    QColor("#C0C6D4"),                       // TextTitle
//    QColor("#6D7C88"),                       // TextTips
//    QColor("#FF5736"),                       // TextWarning
//    QColor("#0082FA"),                       // TextLively
//    QColor("#0056c1"),                       // LightLively
    QColor(0xC0, 0xC6, 0xD4),                     // TextTitle
    QColor(0x6D, 0x7C, 0x88),                     // TextTips
    QColor(0xFF, 0x57, 0x36),                     // TextWarning
    QColor(0x00, 0x82, 0xFA),                     // TextLively
    QColor(0x00, 0x56, 0xc1),                     // LightLively
    QColor(25, 25, 25, CAST_INT(0 * 255)),   // DarkLively
    QColor(25, 25, 25, CAST_INT(0.5 * 255))  // FrameBorder
};


DPalette ExApplicationHelper::standardPalette(DGuiApplicationHelper::ColorType type) const
{
    DPalette pa;
    const QColor *qcolor_list, *dcolor_list;

    if (type == DarkType) {
        qcolor_list = dark_qpalette;
        dcolor_list = dark_dpalette;
    } else {
        qcolor_list = light_qpalette;
        dcolor_list = light_dpalette;
    }

    for (int i = 0; i < DPalette::NColorRoles; ++i) {
        QPalette::ColorRole role = static_cast<QPalette::ColorRole>(i);

        QColor color = qcolor_list[i];
        pa.setColor(DPalette::Active, role, color);
        generatePaletteColor(pa, role, type);
    }

    for (int i = 0; i < DPalette::NColorTypes; ++i) {
        DPalette::ColorType role = static_cast<DPalette::ColorType>(i);

        QColor color = dcolor_list[i];
        pa.setColor(DPalette::Active, role, color);
        generatePaletteColor(pa, role, type);
    }

    return pa;
}

DPalette ExApplicationHelper::palette(const QWidget *widget, const QPalette &base) const
{
    Q_UNUSED(base)

    DPalette palette;

    do {
        // 存在自定义palette时应该根据其自定义的palette获取对应色调的DPalette
        const QPalette &wp = widget->palette();

        palette = standardPalette(toColorType(wp));

        // 关注控件palette改变的事件
        const_cast<QWidget *>(widget)->installEventFilter(const_cast<ExApplicationHelper *>(this));
    } while (false);

    return palette;
}

void ExApplicationHelper::setPalette(QWidget *widget, const DPalette &palette)
{
    // 记录此控件被设置过palette
    widget->setProperty("_d_set_palette", true);
    widget->setPalette(palette);
}

void ExApplicationHelper::resetPalette(QWidget *widget)
{
    widget->setProperty("_d_set_palette", QVariant());
    widget->setAttribute(Qt::WA_SetPalette, false);
}

ExApplicationHelper::ExApplicationHelper() {}

ExApplicationHelper::~ExApplicationHelper() {}

bool ExApplicationHelper::eventFilter(QObject *watched, QEvent *event)
{
    return DGuiApplicationHelper::eventFilter(watched, event);
}
