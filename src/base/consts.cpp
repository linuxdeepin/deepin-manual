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

#include "base/consts.h"

const char kAppName[] = "deepin-manual";
const char kAppProcessName[] = "dman";
const char kAppVersion[] = "2.0.10";
const char kIndexPage[] = DMAN_WEB_DIR "/index.html";

const char kConfigWindowWidth[] = "window_width";
const char kConfigWindowHeight[] = "window_height";
const char kConfigWindowInfo[] = "window_info";
const char kConfigAppList[] = "AppList";

QString GetCacheDir()
{
    const char kAppCacheDir[] = ".cache/deepin/deepin-manual";
    return QDir::home().absoluteFilePath(kAppCacheDir);
}
