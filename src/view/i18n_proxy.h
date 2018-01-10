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

#ifndef DEEPIN_MANUAL_VIEW_I18N_PROXY_H
#define DEEPIN_MANUAL_VIEW_I18N_PROXY_H

#include <QObject>
#include <QHash>

namespace dman {

class I18nProxy : public QObject {
  Q_OBJECT
 public:
  explicit I18nProxy(QObject* parent = nullptr);
  ~I18nProxy() override;

  QHash<QString, QString> getSentences() const;
};

}  // namespace dman

#endif  // DEEPIN_MANUAL_VIEW_I18N_PROXY_H
