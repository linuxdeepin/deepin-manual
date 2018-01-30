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

#include <DApplication>
#include <QCommandLineParser>
#include <QDBusConnection>
#include <QIcon>
#include <QWebEngineProfile>

#include "base/consts.h"
#include "controller/window_manager.h"
#include "resources/themes/images.h"

int main(int argc, char** argv) {
  Dtk::Widget::DApplication::loadDXcbPlugin();

  Dtk::Widget::DApplication app(argc, argv);

  app.setTheme("light");
  app.setAttribute(Qt::AA_EnableHighDpiScaling, true);
  app.setWindowIcon(QIcon(dman::kImageDeepinManual));
  app.setProductIcon(QIcon(dman::kImageDeepinManual));
  app.setOrganizationName("deepin");
  app.setOrganizationDomain("deepin.org");
  app.setApplicationVersion(dman::kAppVersion);
  app.setApplicationName(dman::kAppName);
  app.loadTranslator();
  app.setApplicationDisplayName(QObject::tr(dman::kAppDisplayName));
  app.setApplicationDescription(QObject::tr(
      "Deepin Manual is designed to help users learn deepin and "
          "Deepin applications, providing specific instructions and "
          "function descriptions."));
  app.setApplicationAcknowledgementPage(
      "https://www.deepin.org/acknowledgments/deepin-manual/");

  QWebEngineProfile* profile = QWebEngineProfile::defaultProfile();
  QDir cache_dir = dman::GetCacheDir();
  profile->setCachePath(cache_dir.filePath("cache"));
  profile->setPersistentStoragePath(cache_dir.filePath("storage"));

  dman::WindowManager window_manager;
  window_manager.parseArguments();

  return app.exec();
}
