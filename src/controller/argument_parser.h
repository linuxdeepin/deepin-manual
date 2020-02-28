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

namespace dman {

class ArgumentParser : public QObject
{
    Q_OBJECT
public:
    explicit ArgumentParser(QObject* parent = nullptr);
    ~ArgumentParser() override;

    void parseArguments();
    void openManualsDelay();

signals:
    void openManualAllRequested(const QString& app_name, const QString& key_name,
                                const QString& title_name);
    void openManualRequested(const QString& app_name, const QString& title_name);
    void openManualWithSearchRequested(const QString& app_name, const QString& keyword);
    void searchRequested(const QString& keyword);
    void onNewAppOpen();

private:
    QStringList manuals_;

private slots:
    void onOpenAppRequested(const QString& app_name, const QString& title_name = "");
    void onSearchRequested(const QString& keyword);
};

}  // namespace dman

#endif  // DEEPIN_MANUAL_CONTROLLER_ARGUMENT_PARSER_H
