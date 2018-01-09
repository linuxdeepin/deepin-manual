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
#include <QIcon>

#include "base/consts.h"
#include "controller/window_manager.h"
#include "resources/themes/images.h"

namespace {
//
//QString GetSystemManualDir(const QString& args) {
//  Q_UNUSED(args);
//  return DMAN_MANUAL_DIR;
//}

}  // namespace

int main(int argc, char** argv) {
  Dtk::Widget::DApplication::loadDXcbPlugin();

  Dtk::Widget::DApplication app(argc, argv);

  // TODO(Shaohua): Load theme type from settings.
  app.setTheme("light");

  // FIXME(Shaohua): libqcef does not support HiDPI currently.
  app.setAttribute(Qt::AA_EnableHighDpiScaling, true);
  app.setOrganizationName("deepin");
  app.setOrganizationDomain("deepin.org");
  app.setApplicationVersion(dman::kAppVersion);
  app.setApplicationName(dman::kAppName);
  app.setApplicationDisplayName(QObject::tr(dman::kAppDisplayName));
  app.setApplicationLicense("GPL-3.0");
  app.setApplicationDescription(QObject::tr(
      "Deepin Manual is designed to help users learn deepin and "
          "Deepin applications, providing specific instructions and "
          "function descriptions."));
  app.loadTranslator();
  app.setWindowIcon(QIcon(dman::kImageDeepinManual));
  app.setProductIcon(QIcon(dman::kImageDeepinManual));

  dman::WindowManager window_manager;

  QCommandLineParser parser;
  parser.parse(app.arguments());
  const QStringList position_args = parser.positionalArguments();
  if (position_args.isEmpty()) {
    // Open index page.
    window_manager.openManual("");
  } else {
    // Only parse positional arguments.
    for (const QString& arg : position_args) {
      window_manager.openManual(arg);
    }
  }

  return app.exec();
}
