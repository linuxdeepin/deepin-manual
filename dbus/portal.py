#!/usr/bin/env python3

import os
import sys
import subprocess

from PyQt5.QtCore import pyqtSlot, QCoreApplication, QObject
from PyQt5.QtDBus import QDBusConnection

OpenServ = 'com.deepin.Manual.Open'
OpenPath = '/com/deepin/Manual/Open'
OpenIfce = "com.deepin.Manual.Open"

SearchServ = 'com.deepin.Manual.Search'
SearchPath = '/com/deepin/Manual/Search'
SearchIfce = "com.deepin.Manual.Search"

class Opener(QObject):

    @pyqtSlot(str, name="Open")
    def open(self, arg):
        subprocess.Popen(['dman', arg])

    @pyqtSlot(str, name="ShowManual")
    def show_manual(self, arg):
        self.open(arg)

class Searcher(QObject):

    @pyqtSlot(str, name="ManualExists", result=bool)
    def open(self, arg):
        return os.path.exists("/usr/share/deepin-manual/manual/" + arg)

def reigsterObject(obj, service, path, iface):
    if not sessionBus.registerService(service):
        print("error: cannot register dbus service: " + service)
        return -1

    if not sessionBus.registerObject(path, iface, obj,
        QDBusConnection.ExportAllSlots):
        print("error: cannot register object: " + obj)
        return -1

if __name__ == '__main__':
    app = QCoreApplication(sys.argv)

    sessionBus = QDBusConnection.sessionBus()

    if not sessionBus.isConnected():
        print("error: dbus is not connected")
        sys.exit(-1)

    opener = Opener()
    if reigsterObject(opener, OpenServ, OpenPath, OpenIfce) == -1:
        sys.exit(-1)

    searcher = Searcher()
    if reigsterObject(searcher, SearchServ, SearchPath, SearchIfce) == -1:
        sys.exit(-1)

    sys.exit(app.exec_())
