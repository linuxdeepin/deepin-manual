/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

namespace dman {

extern const char kAppName[];
extern const char* kAppDisplayName;
extern const char kAppVersion[];
extern const char kIndexPage[];

// Get user cache directory.
QString GetCacheDir();

}  // namespace dman

#endif  // DEEPIN_MANUAL_BASE_CONSTS_H
