/*
 * Copyright (C) 2017 ~ $year Deepin Technology Co., Ltd.
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

#include "view/web_event_delegate.h"

#include <qcef_web_page.h>
#include <qcef_web_view.h>

#include <QDebug>

namespace {

enum MenuIds {
  // Normal navigation.
  MenuBack = QCefContextMenu::MENU_ID_USER_FIRST,
  MenuForward,
  MenuReload,
  MenuStop,

  // Editable.
  MenuUndo,
  MenuRedo,
  MenuCut,
  MenuCopy,
  MenuPaste,
  MenuDelete,
  MenuSelectAll,
};

}  // namespace

dman::WebEventDelegate::WebEventDelegate(QObject* parent) : QObject(parent) {

}

bool dman::WebEventDelegate::onBeforeBrowse(const QUrl& url, bool is_redirect) {
  return QCefBrowserEventDelegate::onBeforeBrowse(url, is_redirect);
}

void dman::WebEventDelegate::onBeforeContextMenu(
    QCefWebPage* web_page,
    QCefContextMenu* menu,
    const QCefContextMenuParams& params) {
  Q_UNUSED(web_page);

  auto flags = params.getTypeFlags();
  if (flags & QCEF_CM_FLAG_SELECTION) {
    menu->addItem(MenuIds::MenuCopy,
                  "Copy",
                  true,
                  [](QCefWebPage* page) {
                    page->copy();
                  });

    return;
  }
}

bool dman::WebEventDelegate::onBeforePopup(
    const QUrl& url,
    QCefWindowOpenDisposition disposition) {
  return QCefBrowserEventDelegate::onBeforePopup(url, disposition);
}
