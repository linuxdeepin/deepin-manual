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

#include "view/i18n_proxy.h"

namespace dman {

I18nProxy::I18nProxy(QObject* parent) : QObject(parent) {
}

I18nProxy::~I18nProxy() {
}

QVariantHash I18nProxy::getSentences() const {
  QVariantHash result{
      { "Copy", QObject::tr("Copy") },
      { "System", QObject::tr("System") },
      { "Applications", QObject::tr("Applications") },
      { "NoResult", QObject::tr("Sorry, we couldn't find any results") },
      { "WikiSearch", QObject::tr(
          "Change your keywords and try again, or search it in Deepin Wiki") },
  };
  return result;
}

}  // namespace dman
