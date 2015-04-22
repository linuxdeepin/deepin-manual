# -*- coding: utf-8 -*-


from PyQt5.QtCore import QObject, pyqtSlot, pyqtSignal, QUrl
from PyQt5.QtQuick import QQuickView
import sys, os
import configparser
from pathlib import Path

p = os.path.normpath(os.path.dirname(__file__) + "/../../../DMan")
sys.path.append(p)

from utils import getUILanguage, getDocumentLanguageFor, processMarkdownPath


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


class Bridge(QObject):
    sigShowTooltip = pyqtSignal(str, int, int)

    def __init__(self):
        super().__init__()
        self._configPath = os.path.expanduser("~/.config/deepin-manual/config.ini")
        self._config = configparser.ConfigParser()
        self._config.read(self._configPath, encoding = "utf-8")
        try:
            (Path(os.path.expanduser("~/.config/deepin-manual"))).mkdir(parents = True)
        except FileExistsError:
            pass
        Path(self._configPath).touch()
        mdUrl = sys.argv[-1]
        self._dmanInfo = processMarkdownPath(mdUrl, getDocumentLanguageFor)
        self._tooltipView = TooltipView(self)

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
            self._config.set("general", "isFirstRun", "True")
            self._writeConfig()
        return yes

    def _writeConfig(self):
        with open(self._configPath, 'w', encoding = "utf-8") as f:
            self._config.write(f)


def export_objects():
    bridge = Bridge()
    return [
        dict(name = "bridge", obj = bridge),
    ]
