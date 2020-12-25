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

#ifndef DEEPIN_MANUAL_CONTROLLER_ARGUMENT_PARSER_H
#define DEEPIN_MANUAL_CONTROLLER_ARGUMENT_PARSER_H

#include <QObject>

class ArgumentParser : public QObject
{
    Q_OBJECT
public:
    explicit ArgumentParser(QObject *parent = nullptr);
    ~ArgumentParser() override;
    //解析传入参数
    bool parseArguments();
    void openManualsDelay();

signals:
    void openManualRequested(const QString &app_name, const QString &title_name);
    void openManualWithSearchRequested(const QString &app_name, const QString &keyword);
    void newAppOpen();

private slots:
    //打开对应模块文案槽函数
    void onOpenAppRequested(const QString &app_name, const QString &title_name = "");
    //查找关键字槽函数
    void onSearchRequested(const QString &keyword);

private:
    QString curManual;
    bool bIsDbus;
};

#endif // DEEPIN_MANUAL_CONTROLLER_ARGUMENT_PARSER_H
