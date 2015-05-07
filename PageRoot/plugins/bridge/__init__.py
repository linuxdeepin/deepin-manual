# -*- coding: utf-8 -*-


from PyQt5.QtCore import QObject, pyqtSlot, pyqtSignal, QUrl
from PyQt5.QtQuick import QQuickView
from PyQt5.QtGui import QDesktopServices
from PyQt5.QtWidgets import QApplication
import sys, os
import configparser
from pathlib import Path
try:
    import jieba
except ImportError:
    jieba = None
import threading
from multiprocessing.connection import Listener

p = os.path.normpath(os.path.dirname(__file__) + "/../../../DMan")
sys.path.append(p)

from utils import getUILanguage, getDocumentLanguageFor, processMarkdownPath,\
    getSocketPath, getConfigPath, getLockPath


class TooltipView(QQuickView):
    sigShowTooltip = pyqtSignal(str, int, int)

    def __init__(self, parent):
        super().__init__(None)
        parent.sigShowTooltip.connect(self.showTooltip)
        self.rootContext().setContextProperty("view", self)
        self.setSource(QUrl.fromLocalFile(os.path.dirname(__file__) + "/TooltipView.qml"))

    @pyqtSlot(str, int, int, result = "void*")
    def showTooltip(self, text, x, y):
        self.sigShowTooltip.emit(text, x, y)


class CommandlineWatcher(QObject):
    def __init__(self, parent):
        super().__init__(parent)
        self._listener = threading.Thread(target = self.listenerThread,
                                          daemon = True,
                                          name = "instance communication")
        self._listener.start()

    def listenerThread(self):
        # clean if previous run crashes
        socket = getSocketPath(sys.argv[-1])
        try:
            os.remove(socket)
        except FileNotFoundError:
            pass
        with Listener(socket, "AF_UNIX") as listener:
            while True:
                with listener.accept() as conn:
                    payload = conn.recv()
                    win = QApplication.topLevelWindows()[1]
                    win.raise_()


class Bridge(QObject):
    sigShowTooltip = pyqtSignal(str, int, int)

    def __init__(self):
        super().__init__()
        self._configPath = getConfigPath()
        self._config = configparser.ConfigParser()
        self._config.read(self._configPath, encoding = "utf-8")
        mdUrl = sys.argv[-1]
        self._dmanInfo = processMarkdownPath(mdUrl, getDocumentLanguageFor)
        self._lockPath = getLockPath(mdUrl)
        self._tooltipView = TooltipView(self)
        self._commandlineWatcher = CommandlineWatcher(self)
        QApplication.instance().aboutToQuit.connect(self.cleanup)

    @pyqtSlot(result = "QStringList")
    def uiLangs(self):
        return [getUILanguage()]

    @pyqtSlot(result = str)
    def mdDir(self):
        return self._dmanInfo.dir

    @pyqtSlot(result = str)
    def lang(self):
        return self._dmanInfo.lang

    @pyqtSlot(result = bool)
    def debug(self):
        return not not os.environ.get("DEBUG", None)

    @pyqtSlot(bool)
    def setCompactMode(self, on):
        if not self._config.has_section("general"):
            self._config.add_section("general")
        self._config.set("general", "isCompactMode", "True" if on else "False")
        self._writeConfig()

    @pyqtSlot(result = bool)
    def isCompactMode(self):
        return self._config.getboolean("general", "isCompactMode",
                                       fallback = False)

    @pyqtSlot(result = bool)
    def isFirstRun(self):
        yes = self._config.getboolean("general", "isFirstRun",
                                      fallback = True)
        if yes:
            if not self._config.has_section("general"):
                self._config.add_section("general")
            self._config.set("general", "isFirstRun", "False")
            self._writeConfig()
        return yes

    def _writeConfig(self):
        with open(self._configPath, 'w', encoding = "utf-8") as f:
            self._config.write(f)

    @pyqtSlot(str, result = "QStringList")
    def getWordCutting(self, text: str):
        if self.lang() == "zh_CN" and jieba:
            return list(jieba.cut(text))
        else:
            return []

    @pyqtSlot(str)
    def openExternalBrowser(self, url: str):
        QDesktopServices.openUrl(QUrl("http://bbs.deepin.org/"))

    @pyqtSlot()
    def cleanup(self):
        try:
            os.remove(self._lockPath)
        except OSError:
            pass


def export_objects():
    bridge = Bridge()
    return [
        dict(name = "bridge", obj = bridge),
    ]
