# -*- coding: utf-8 -*-

from PyQt5.QtCore import QUrl, Qt, pyqtSlot, pyqtSignal, pyqtProperty
from PyQt5.QtQuick import QQuickView


class QmlError(Exception):
    def __init__(self, errors: "list<QQmlError>"):
        output = "".join(map(lambda generator: self.formatOneError(*generator),
                             enumerate(errors)))
        super().__init__(output)

    @staticmethod
    def formatOneError(i: "Error Sequence", error):
        output = """\n\tError({i}) {url}, line {line}
\t\t{description}
""".format(i = i, url = error.url().url(), line = error.line(), column = error.column(),
           description = error.description())
        return output


class StrictQmlError(QmlError):
    """Non-fatal Qml Error, raise them anyway."""


class QmlLoadingError(QmlError):
    """Qml cannot be loaded."""

from QmlResizable import QmlResizable


class StrictQuickView(QmlResizable):
    def __init__(self, parent = None):
        super().__init__(parent)
        self.engine().warnings.connect(self.strictWarn)
        self.engine().setOutputWarningsToStandardError(False)  # let .strictWarn handle it!

        self.statusChanged.connect(self.slotStatusChanged)

    def strictWarn(self, errors):
        raise StrictQmlError(errors)

    def slotStatusChanged(self, status: "QQuickView::Status"):
        if status == QQuickView.Error:
            raise QmlLoadingError(self.errors())


class MainView(StrictQuickView):
    signalMaximize = pyqtSignal()
    signalMinimize = pyqtSignal()
    signalClose = pyqtSignal()

    signalMaximized = pyqtSignal(bool)

    def __init__(self, mdUrl, debugMode):
        super().__init__(None)
        self.rootContext().setContextProperty("DManBridge", self)
        self.setResizeMode(QQuickView.SizeRootObjectToView)
        self._isBeingDragged = False
        self._isResizing = False
        self._dragOffset = None
        self.setFlags(Qt.FramelessWindowHint | Qt.Window)
        self._mdUrl = mdUrl
        self._debugMode = debugMode
        self.setSource(QUrl.fromLocalFile("./qml/index.qml"))
        self.signalMaximize.connect(self.slotMaximize)
        self.signalMinimize.connect(self.slotMinimize)
        self.signalClose.connect(self.slotClose)

        self.setMinimumHeight(600)
        self.setMinimumWidth(946)
        self.setDragHeight(30)
        self.setResizePaddings(8, 8, 8, 8)

    @pyqtProperty(str)
    def mdUrl(self):
        return self._mdUrl

    @pyqtProperty(bool)
    def debugMode(self):
        return self._debugMode

    @pyqtSlot()
    def slotMaximize(self):
        if int(self.windowState()) & Qt.WindowMaximized:
            self.showNormal()
        else:
            self.showMaximized()

    @pyqtSlot()
    def slotMinimize(self):
        self.showMinimized()

    @pyqtSlot()
    def slotClose(self):
        self.close()
