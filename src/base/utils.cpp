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
const int langCount = 2;
QString languageArr[][langCount] = {
                                    //dde model
                                    {"License activator", "授权管理"},
                                    {"controlcenter","控制中心"},
                                    {"accounts","账户设置"},
                                    {"cloudsync","网络账户"},
                                    {"display","显示设置"},
                                    {"defapp","默认程序设置"},
                                    {"personalization","个性化设置"},
                                    {"network","网络设置"},
                                    {"sound","声音设置"},
                                    {"bluetooth","蓝牙设置"},
                                    {"datetime","日期和时间"},
                                    {"power","电源管理"},
                                    {"mouse","鼠标和触控板"},
                                    {"tablet","数位板"},
                                    {"keyboard","键盘和语言"},
                                    {"update","系统更新"},
                                    {"systeminfo","系统信息"},
                                    {"commoninfo","通用设置"}};

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

QString Utils::getConfigPath()
{
    QDir dir(QDir(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first())
                 .filePath(qApp->organizationName()));

    return dir.filePath(qApp->applicationName());
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

QString Utils::translateTitle(const QString &titleUS)
{
    QString strRet = titleUS;
    if (QLocale::system().name() == "zh_CN") {
        int nCount = sizeof(languageArr) / sizeof(languageArr[0]);
        for (int i = 0; i < nCount; i++) {
            if (languageArr[i][0] == titleUS) {
                strRet = languageArr[i][1];
                break;
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

static QColor light_qpalette[QPalette::NColorRoles] {
    QColor("#000000"),                           // WindowText
    QColor("#e5e5e5"),                           // Button
    QColor("#e6e6e6"),                           // Light
    QColor("#e5e5e5"),                           // Midlight
    QColor("#e3e3e3"),                           // Dark
    QColor("#e4e4e4"),                           // Mid
    QColor("#414d68"),                           // Text
    Qt::black,                                   // BrightText
    QColor("#414d68"),                           // ButtonText
    Qt::white,                                   // Base
    QColor("#f8f8f8"),                           // Window
    QColor(0, 0, 0, CAST_INT(0.05 * 255)),       // Shadow
    QColor("#0081ff"),                           // Highlight
    Qt::white,                                   // HighlightedText
    QColor("#0082fa"),                           // Link
    QColor("#ad4579"),                           // LinkVisited
    QColor(0, 0, 0, CAST_INT(0.03 * 255)),       // AlternateBase
    Qt::white,                                   // NoRole
    QColor(255, 255, 255, CAST_INT(0.8 * 255)),  // ToolTipBase
    Qt::black                                    // ToolTipText
};

static QColor dark_qpalette[QPalette::NColorRoles] {
    QColor("#ffffff"),                        // WindowText
    QColor("#444444"),                        // Button
    QColor("#484848"),                        // Light
    QColor("#474747"),                        // Midlight
    QColor("#414141"),                        // Dark
    QColor("#434343"),                        // Mid
    QColor("#c0c6d4"),                        // Text
    Qt::white,                                // BrightText
    QColor("#c0c6d4"),                        // ButtonText
    QColor("#282828"),                        // Base
    QColor("#252525"),                        // Window
    QColor(0, 0, 0, CAST_INT(0.05 * 255)),    // Shadow
    QColor("#0081ff"),                        // Highlight
    QColor("#F1F6FF"),                        // HighlightedText
    QColor("#0082fa"),                        // Link
    QColor("#ad4579"),                        // LinkVisited
    QColor(0, 0, 0, CAST_INT(0.05 * 255)),    // AlternateBase
    Qt::black,                                // NoRole
    QColor(45, 45, 45, CAST_INT(0.8 * 255)),  // ToolTipBase
    QColor("#c0c6d4")                         // ToolTipText
};

static QColor light_dpalette[DPalette::NColorTypes] {
    QColor(),                                    // NoType
    QColor(255, 255, 255, CAST_INT(0.2 * 255)),  // ItemBackground
    QColor("#001A2E"),                           // TextTitle
    QColor("#8AA1B4"),                           // TextTips
    QColor("#FF5736"),                           // TextWarning
    QColor("#0082FA"),                           // TextLively
    QColor("#25b7ff"),                           // LightLively
    QColor(235, 235, 235, CAST_INT(0 * 255)),    // DarkLively
    QColor(235, 235, 235, CAST_INT(0 * 255))     // FrameBorder
};

static QColor dark_dpalette[DPalette::NColorTypes] {
    QColor(),                                // NoType
    QColor(25, 25, 25, CAST_INT(0 * 255)),   // ItemBackground
    QColor("#C0C6D4"),                       // TextTitle
    QColor("#6D7C88"),                       // TextTips
    QColor("#FF5736"),                       // TextWarning
    QColor("#0082FA"),                       // TextLively
    QColor("#0056c1"),                       // LightLively
    QColor(25, 25, 25, CAST_INT(0 * 255)),   // DarkLively
    QColor(25, 25, 25, CAST_INT(0.5 * 255))  // FrameBorder
};

DPalette ExApplicationHelper::standardPalette(DGuiApplicationHelper::ColorType type) const
{
    DPalette *pa;
    const QColor *qcolor_list, *dcolor_list;

    if (type == DarkType) {
        pa = new DPalette();
        qcolor_list = dark_qpalette;
        dcolor_list = dark_dpalette;
    } else {
        pa = new DPalette();
        qcolor_list = light_qpalette;
        dcolor_list = light_dpalette;
    }

    for (int i = 0; i < DPalette::NColorRoles; ++i) {
        QPalette::ColorRole role = static_cast<QPalette::ColorRole>(i);

        QColor color = qcolor_list[i];
        pa->setColor(DPalette::Active, role, color);
        generatePaletteColor(*pa, role, type);
    }

    for (int i = 0; i < DPalette::NColorTypes; ++i) {
        DPalette::ColorType role = static_cast<DPalette::ColorType>(i);

        QColor color = dcolor_list[i];
        pa->setColor(DPalette::Active, role, color);
        generatePaletteColor(*pa, role, type);
    }

    return *const_cast<const DPalette *>(pa);
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
