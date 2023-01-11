// SPDX-FileCopyrightText: 2017 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
