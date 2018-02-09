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

#include <DApplication>
#include <LogManager.h>
#include <QCommandLineParser>
#include <QDBusConnection>
#include <QDebug>
#include <QIcon>
#include <QTimer>
#include <QWebEngineProfile>

#include "base/consts.h"
#include "controller/argument_parser.h"
#include "controller/window_manager.h"
#include "resources/themes/images.h"

int main(int argc, char** argv) {
  qputenv("DXCB_FAKE_PLATFORM_NAME_XCB", "true");
  qputenv("DXCB_REDIRECT_CONTENT", "true");

  const char* kScaleFactorName = "QT_SCALE_FACTOR";
  const QString curr_scale_factor = qgetenv(kScaleFactorName);
  if (!curr_scale_factor.isEmpty()) {
    const double factor = curr_scale_factor.toDouble();
    QByteArray factor_bytes;
    factor_bytes.setNum(qRound(factor));
    qputenv(kScaleFactorName, factor_bytes);
  }

  // Load custom version of dxcb plugin.
  // TODO(Shaohua): Remove this when qt5dxcb-plugin updated to 1.1.7
  if (QFile("/usr/share/deepin-manual/plugins/libdxcb.so").exists()){
    qDebug() << "load dxcb from local path";
    qputenv("QT_QPA_PLATFORM_PLUGIN_PATH", "/usr/share/deepin-manual/plugins");
  }

  Dtk::Widget::DApplication::loadDXcbPlugin();

  Dtk::Widget::DApplication app(argc, argv);

  app.setTheme("light");
  app.setAttribute(Qt::AA_EnableHighDpiScaling, true);
  app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);
  app.setWindowIcon(QIcon(dman::kImageDeepinManual));
  app.setProductIcon(QIcon(dman::kImageDeepinManual));
  app.setOrganizationName("deepin");
  app.setOrganizationDomain("deepin.org");
  app.setApplicationVersion(dman::kAppVersion);
  app.setApplicationName(dman::kAppName);
  app.loadTranslator();
  app.setApplicationDisplayName(QObject::tr("Deepin Manual"));
  app.setApplicationDescription(QObject::tr(
      "Deepin Manual is designed to help users learn deepin and "
          "Deepin applications, providing specific instructions and "
          "function descriptions."));
  app.setApplicationAcknowledgementPage(
      "https://www.deepin.org/acknowledgments/deepin-manual/");

  Dtk::Core::DLogManager::registerFileAppender();
  Dtk::Core::DLogManager::registerConsoleAppender();

  QWebEngineProfile* profile = QWebEngineProfile::defaultProfile();
  QDir cache_dir = dman::GetCacheDir();
  profile->setCachePath(cache_dir.filePath("cache"));
  profile->setPersistentStoragePath(cache_dir.filePath("storage"));

  dman::ArgumentParser argument_parser;

  if (argument_parser.parseArguments()) {
    // Exit process after 1000ms.
    QTimer::singleShot(1000, [&]() {
      app.quit();
    });
    return app.exec();
  } else {
    dman::WindowManager window_manager;
    QObject::connect(&argument_parser,
                     &dman::ArgumentParser::openManualRequested,
                     &window_manager, &dman::WindowManager::openManual);
    // Send openManualRequested() signals after slots connected.
    argument_parser.openManualsDelay();
    return app.exec();
  }
}
