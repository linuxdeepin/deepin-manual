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

#ifndef DEEPIN_MANUAL_BASE_CONSTS_H
#define DEEPIN_MANUAL_BASE_CONSTS_H

#include <QDir>

extern const char kAppName[];
extern const char kAppProcessName[];
extern const char kAppVersion[];
extern const char kIndexPage[];

extern const char kConfigWindowWidth[];
extern const char kConfigWindowHeight[];
extern const char kConfigWindowInfo[];
extern const char kConfigAppList[];

// 获取用户缓存目录
QString GetCacheDir();

#endif // DEEPIN_MANUAL_BASE_CONSTS_H
