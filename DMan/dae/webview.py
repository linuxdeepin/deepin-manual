#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (C) 2011~2014 Deepin, Inc.
#               2011~2014 Kaisheng Ye
#
# Author:     Kaisheng Ye <kaisheng.ye@gmail.com>
# Maintainer: Kaisheng Ye <kaisheng.ye@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import os
import re

import dae.assets as assets
from dae.webpage import WebPage

from PyQt5.QtWebKitWidgets import QWebView, QWebPage
from PyQt5.Qt import Qt, QWebSettings, QDrag, QUrl, pyqtSlot
from PyQt5.QtGui import QRegion, QDropEvent

class WebView(QWebView):
    def __init__(self, parent, url = ''):
        super(WebView, self).__init__(parent)
        self.draging = False
        self.assets = assets
        self.window = parent
        self.drag = QDrag(self)
        self.dragStartPos = None
        self.webPage = WebPage()
        if not assets.manifest['debug']:
            actions = [
                    "OpenLink",
                    "OpenLinkInNewWindow",
                    "OpenLinkInThisWindow",
                    "OpenFrameInNewWindow",
                    "DownloadLinkToDisk",
                    "CopyLinkToClipboard",
                    "OpenImageInNewWindow",
                    "DownloadImageToDisk",
                    "CopyImageToClipboard",
                    "CopyImageUrlToClipboard",
                    "Back",
                    "Forward",
                    "Reload",
                    "ReloadAndBypassCache",
                    ]
            for action in actions:
                action_id = getattr(QWebPage, action)
                q_action = self.webPage.action(action_id)
                if q_action:
                    q_action.setVisible(False)
        self.setPage(self.webPage)
        self.mainFrame = self.page().mainFrame()
        self.setAttribute(Qt.WA_DeleteOnClose, True)
        self.titleChanged.connect(self.window.setWindowTitle)
        webSettings = self.settings()
        webSettings.setDefaultTextEncoding("utf-8")
        # webSettings.enablePersistentStorage(assets.fs.dataPath())
        # webSettings.setAttribute(QWebSettings.PluginsEnabled, True)
        # webSettings.setAttribute(QWebSettings.DnsPrefetchEnabled, True)
        webSettings.setAttribute(QWebSettings.XSSAuditingEnabled, True)
        webSettings.setAttribute(QWebSettings.CSSGridLayoutEnabled, True)
        webSettings.setAttribute(QWebSettings.DeveloperExtrasEnabled, assets.manifest['debug'])
        webSettings.setAttribute(QWebSettings.JavascriptCanOpenWindows, True)
        webSettings.setAttribute(QWebSettings.JavascriptCanCloseWindows, True)
        webSettings.setAttribute(QWebSettings.JavascriptCanAccessClipboard, True)
        webSettings.setAttribute(QWebSettings.LocalContentCanAccessFileUrls, True)
        webSettings.setAttribute(QWebSettings.LocalContentCanAccessRemoteUrls, False)
        self.mainFrame.javaScriptWindowObjectCleared.connect(self.setJavaScriptObject)
        self.mainFrame.iconChanged.connect(self.changeIcon)
        self.load(url)

    def setRoundedCorners(self, rect, radius):
        region = QRegion(rect, QRegion.Rectangle)

        # top left
        _round = QRegion(rect.x(), rect.y(), 2*radius, 2*radius, QRegion.Ellipse)
        _corner  = QRegion(rect.x(), rect.y(), radius, radius, QRegion.Rectangle)
        region = region.subtracted(_corner.subtracted(_round))

        # top right
        _round = QRegion(rect.x() + rect.width() - 2*radius, rect.y(), 2*radius, 2*radius, QRegion.Ellipse)
        _corner  = QRegion(rect.x() + rect.width() - radius, rect.y(), radius, radius, QRegion.Rectangle)
        region = region.subtracted(_corner.subtracted(_round))

        # bottom left
        _round = QRegion(rect.x(), rect.y() + rect.height() - 2*radius, 2*radius, 2*radius, QRegion.Ellipse)
        _corner  = QRegion(rect.x(), rect.y() + rect.height() - radius, radius, radius, QRegion.Rectangle)
        region = region.subtracted(_corner.subtracted(_round))

        # bottom right
        _round = QRegion(rect.x() + rect.width() - 2*radius, rect.y() + rect.height() - 2*radius, 2*radius, 2*radius, QRegion.Ellipse)
        _corner  = QRegion(rect.x() + rect.width() - radius, rect.y() + rect.height() - radius, radius, radius, QRegion.Rectangle)
        region = region.subtracted(_corner.subtracted(_round))

        self.setMask(region)

    def load(self, url = ''):
        p = re.compile('(^file:\/\/)|(^http:\/\/)|(^https:\/\/)|(^data:)')
        if url and p.match(url) == None:
            url = QUrl.fromLocalFile(os.path.abspath(url))
        else:
            url = QUrl(url)
        super(WebView, self).load(url)

    def eval(self, javaScript):
        return self.mainFrame.evaluateJavaScript(javaScript)

    def changeIcon(self):
        self.window.setWindowIcon(self.mainFrame.icon())
        # self.parentWidget().setWindowIcon(QWebSettings.iconForUrl(self.url()))

    def setJavaScriptObject(self):
        self.mainFrame.addToJavaScriptWindowObject('DAE', self.window.api)
        self.mainFrame.addToJavaScriptWindowObject('DAE_app', self.window)
        # self.mainFrame.addToJavaScriptWindowObject('DAE_fs', assets.fs)
        # self.mainFrame.addToJavaScriptWindowObject('DAE_sys', assets.sys)
        # self.mainFrame.addToJavaScriptWindowObject('DAE_codec', assets.codec)

        for plugin in assets.pluginobjects:
            name = plugin["name"]
            obj = plugin["obj"]
            if hasattr(obj, "enable"):
                obj.enable(self)

            self.mainFrame.addToJavaScriptWindowObject(name, obj)
            self.window.api.pluginListAppend(name)

        init_js_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), "init.js")
        with open(init_js_path) as f:
            self.eval(f.read())

    def mousePressEvent(self, event):
        super(WebView, self).mousePressEvent(event)
        event.ignore()

    def mouseMoveEvent(self, event):
        if not self.window.dragParams["draging"]:
            super(WebView, self).mouseMoveEvent(event)
        event.ignore()

    def mouseReleaseEvent(self, event):
        super(WebView, self).mouseReleaseEvent(event)
        event.ignore()

    def close(self):
        self.stop()
        self.load()
        self.page().close()
        self.eval('DAE = null')
        super(WebView, self).close()

    @pyqtSlot(QDropEvent)
    def dropEvent(self, qDropEvent: QDropEvent):
        pass
