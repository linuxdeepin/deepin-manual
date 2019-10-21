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

/**
 * Generate search entry and save into sqlite database.
 * Call this program while build deb packages.
 */

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "base/command.h"
#include "controller/search_db.h"

int main(int argc, char** argv) {
  qputenv("LANG", "en_US.UTF8");
  qputenv("LC_ALL", "en_US.UTF8");
  qputenv("LANGUAGE", "en_US");

  QCoreApplication app(argc, argv);

  QDir::setCurrent(DMAN_SEARCH_WORK_DIR);

  dman::SearchDb db;
  db.initDb(DMAN_SEARCH_ORIG_DB);
  db.initSearchTable();

  for (const QString& locale : { "zh_CN", "en_US" }) {
    for (const QString& app_name :
        QDir(DMAN_ORIG_MANUAL_DIR).entryList(QDir::NoDotAndDotDot | QDir::Dirs)) {
      const QString md_file = QStringList{
          DMAN_ORIG_MANUAL_DIR,
          app_name,
          locale,
          "index.md"
      }.join(QDir::separator());
      if (!QFileInfo(md_file).isFile()) {
        qWarning() << md_file << "not found";
        continue;
      }

      QString out, err;
      const QStringList cmd = {"./toSearchIndex.js", "-f", md_file};
      const bool ok = dman::SpawnCmd("./node_modules/.bin/node", cmd, out, err);
      if (!ok) {
        qWarning() << err << ok << md_file;
        continue;
      }

      // Parse json
      QJsonDocument document = QJsonDocument::fromJson(out.toLocal8Bit());
      if (!document.isArray()) {
        qWarning() << md_file << "Invalid json object:" << out;
        continue;
      }

      const QJsonArray array = document.array();
      QStringList anchors;
      QStringList anchorIdList;
      QStringList contents;
      bool invalid_entry = false;
      for (const QJsonValue& value : array) {
        if (!value.isArray()) {
          invalid_entry = true;
          qWarning() << md_file << "is not an array:" << value;
          break;
        }

        const QJsonArray anchor = value.toArray();
        const QString id = anchor.at(0).toString();
        anchorIdList.append(id);
        const QString title = anchor.at(1).toString();
        anchors.append(title);
        const QString content = anchor.at(2).toString();
        contents.append(content);
      }

      if (!invalid_entry) {
        db.addSearchEntry(app_name, locale, anchors, anchorIdList, contents);
      }
    }
  }
}
