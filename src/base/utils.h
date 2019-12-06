/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
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

#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QHash>
#include <QWidget>

#include <DPalette>
#include <DApplicationHelper>

#define dApp (static_cast<DApplication *>(QCoreApplication::instance()))

DWIDGET_USE_NAMESPACE

class Utils : public QObject
{
    Q_OBJECT

public:
    Utils(QObject *parent = nullptr);
    ~Utils();

    static Utils* instance();

    enum FontType {
        SourceHanSansMedium,
        SourceHanSansNormal,
        DefautFont
    };

    static QHash<QString, QPixmap> m_imgCacheHash;
    static QHash<QString, QString> m_fontNameCache;

    QHash<WId, unsigned long> m_activeTimestampHash;

    static QString getQssContent(const QString &filePath);
    static QString getConfigPath();
    static bool isFontMimeType(const QString &filePath);
    static QString suffixList();
    static QPixmap renderSVG(const QString &filePath, const QSize &size);
    static QString loadFontFamilyByType(FontType fontType);
    static QFont loadFontBySizeAndWeight(QString fontFamily, int fontSize, int fontWeight);
    static QString fromSpecialEncoding(const QString &inputStr);
};

class ExApplicationHelper : public DGuiApplicationHelper
{
    Q_OBJECT

public:
    static ExApplicationHelper *instance();

    DPalette standardPalette(DGuiApplicationHelper::ColorType type) const;
    DPalette palette(const QWidget *widget, const QPalette &base = QPalette()) const;
    void setPalette(QWidget *widget, const DPalette &palette);
    void resetPalette(QWidget *widget);

private:
    ExApplicationHelper();
    ~ExApplicationHelper() override;

    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif
