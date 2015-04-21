# -*- coding: utf-8 -*-


from PyQt5.QtCore import QObject, pyqtSlot, pyqtSignal, QUrl
from PyQt5.QtQuick import QQuickView
import sys, os

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


def export_objects():
    bridge = Bridge()
    return [
        dict(name = "bridge", obj = bridge),
    ]
