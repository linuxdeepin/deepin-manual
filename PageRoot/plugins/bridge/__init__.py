# -*- coding: utf-8 -*-


from PyQt5.QtCore import QObject, pyqtSlot
import sys, os

p = os.path.normpath(os.path.dirname(__file__) + "/../../../DMan")
sys.path.append(p)

from utils import getUILanguage, getDocumentLanguageFor, processMarkdownPath


class Bridge(QObject):
    def __init__(self):
        super().__init__()
        mdUrl = sys.argv[-1]
        self._dmanInfo = processMarkdownPath(mdUrl, getDocumentLanguageFor)

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
