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

import dae.assets as assets
import webbrowser
from PyQt5.Qt import QMessageBox
# from PyQt5.QtNetwork import QNetworkDiskCache
from PyQt5.QtWebKitWidgets import QWebPage, QWebInspector

class WebPage(QWebPage):
    def __init__(self):
        super(WebPage, self).__init__()
        self.inspector = QWebInspector()
        self.inspector.setPage(self)
        self.inspector.resize(1024, 400)
        # diskCache = QNetworkDiskCache(self)
        # diskCache.setCacheDirectory(assets.fs.dataPath() + '/Cache')
        # self.networkAccessManager().setCache(diskCache)
        # self.networkAccessManager().setCookieJar(assets.dataJar)

    def javaScriptConsoleMessage(self, messgae, lineNumber, sourceId):
        if assets.manifest['debug']:
            print("==>[console]: %s @%s: %s" % (sourceId, lineNumber, messgae))

    def userAgentForUrl(self, url):
        return "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/38.0.2125.101 Safari/537.36"

    def acceptNavigationRequest(self, frame, request, type):
        if(type == QWebPage.NavigationTypeLinkClicked):
            url = request.url().toString()
            if(frame == self.mainFrame()):
                    self.view().load(url)
                    return False
            elif frame == None:
                    # self.createWindow(QWebPage.WebBrowserWindow, url)
                    webbrowser.open(request.url().toString())
                    return False
        return QWebPage.acceptNavigationRequest(self, frame, request, type)

    # def downloadRequested(self, request):
    #         print(request)

    def findText(self, text):
        return super(WebPage, self).findText(text, QWebPage.FindBackward)

    def showInspector(self):
        self.inspector.show()
        self.inspector.activateWindow()

    def hideInspector(self):
        self.inspector.close()

    def createWindow(self, type, url = None):
        from dae.window import Window
        window = Window(self.view().parentWidget(), url, isDialog = (type == QWebPage.WebModalDialog))
        return window.webView.page()

    def javaScriptAlert(self, frame, msg):
        QMessageBox.information(self.view().parentWidget(), None, msg)

    def javaScriptConfirm(self, frame, msg):
        return QMessageBox.question(self.view().parentWidget(), None, msg) == QMessageBox.Yes

    # There is a bug in PyQt
    # def javaScriptPrompt(self, frame, msg, defaultValue):
    #         result = QInputDialog.getText(self.view().parentWidget(), None, msg)
    #         return (result[1], result[0])

    def close(self):
        self.hideInspector()
        # assets.dataJar.save()
