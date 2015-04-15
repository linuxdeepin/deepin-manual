# -*- coding: utf-8 -*-

from enum import IntEnum, unique
from collections import namedtuple

from PyQt5.Qt import QPoint, QRect, Qt, QUrl, pyqtSignal, pyqtSlot, QObject, QTimer, pyqtProperty
from PyQt5.QtCore import QEvent
from PyQt5.QtGui import QGuiApplication
from PyQt5.QtQuick import QQuickView
from PyQt5.QtQml import QQmlApplicationEngine


ResizingPaddings = namedtuple("ResizingPaddings", ["top", "right", "bottom", "left"])

@unique
class ResizingEdges(IntEnum):
    Not = 0
    Top = 1
    Right = 2
    Bottom = 4
    Left = 8
    TopLeft = Top | Left
    TopRight = Top | Right
    BottomLeft = Bottom | Left
    BottomRight = Bottom | Right


def cursorInDragRegion(pos: QPoint, height: int) -> bool:
    return 0 <= pos.y() <= height


def cursorInResizeRegion(pos: QPoint, area: QRect, paddings: ResizingPaddings) -> ResizingEdges:
    result = ResizingEdges.Not
    if 0 <= pos.y() <= paddings.top:
        result |= ResizingEdges.Top

    if 0 <= pos.x() <= paddings.left:
        result |= ResizingEdges.Left

    if area.width() - paddings.right <= pos.x() <= area.width():
        result |= ResizingEdges.Right

    if area.height() - paddings.bottom <= pos.y() <= area.height():
        result |= ResizingEdges.Bottom

    return result


def edgeToCursor(edge: ResizingEdges) -> int:
    if edge == ResizingEdges.Not:
        return Qt.ArrowCursor
    elif edge in (ResizingEdges.Top, ResizingEdges.Bottom):
        return Qt.SizeVerCursor
    elif edge in (ResizingEdges.Left, ResizingEdges.Right):
        return Qt.SizeHorCursor
    elif edge in (ResizingEdges.TopLeft, ResizingEdges.BottomRight):
        return Qt.SizeFDiagCursor
    elif edge in (ResizingEdges.TopRight, ResizingEdges.BottomLeft):
        return Qt.SizeBDiagCursor
    return Qt.ArrowCursor


def bound(minimum: int, value: int, maximum: int):
    if minimum > maximum:
        raise ValueError("minimum > maximum")

    if minimum <= value <= maximum:
        return value

    if minimum > value:
        return minimum

    if value > maximum:
        return maximum


def resizeTo(*, resizeEdge: ResizingEdges, geo: QRect, pos: QPoint, gPos: QPoint, prevGeo: QRect, prevPos: QPoint,
             minHeight: int, maxHeight: int, minWidth: int, maxWidth: int):
    if resizeEdge & ResizingEdges.Top:
        newHeight = prevGeo.height() + prevGeo.y() - geo.y()
        geo.setHeight(bound(minHeight, newHeight, maxHeight))
        if gPos.y() < geo.y():
            geo.setY(gPos.y())
    elif resizeEdge & ResizingEdges.Bottom:
        newHeight = prevGeo.height() + pos.y() - prevPos.y()
        geo.setHeight(bound(minHeight, newHeight, maxHeight))

    if resizeEdge & ResizingEdges.Left:
        newWidth = prevGeo.width() + prevGeo.x() - geo.x()
        geo.setWidth(bound(minWidth, newWidth, maxWidth))
        if gPos.x() < geo.x():
            geo.setX(gPos.x())
    elif resizeEdge & ResizingEdges.Right:
        newWidth = prevGeo.width() + pos.x() - prevPos.x()
        geo.setWidth(bound(minWidth, newWidth, maxWidth))
    return geo


class FakeFrame(QQmlApplicationEngine):
    FrameGeometryUpdated = pyqtSignal()

    def __init__(self, parent):
        # Make Qt not aware of FakeFrame has a parent
        super().__init__(None)
        self.rootContext().setContextProperty("frameView", self)
        self.__parent = parent
        self.__parent.ResizeStart.connect(self.onResizeStart)
        self.__parent.ResizeEnd.connect(self.onResizeEnd)
        self.__parent.ResizeSizeUpdated.connect(self.onResizeSizeUpdated)
        self._frameGeometry = QRect(0, 0, 0, 0)
        self.objectCreated.connect(self.onCreated)
        self._maskWin = None
        self.load(QUrl.fromLocalFile("./qml/ResizeFrame.qml"))

    @pyqtSlot()
    def onResizeStart(self):
        if self._maskWin:
            self._maskWin.show()

    @pyqtSlot()
    def onResizeEnd(self):
        if self._maskWin:
            self._maskWin.hide()

    @pyqtSlot(QRect)
    def onResizeSizeUpdated(self, geo: QRect):
        if self._maskWin:
            geo.setHeight(geo.height() + 2)
            geo.setWidth(geo.width() + 2)
            geo.setLeft(geo.left() - 1)
            geo.setTop(geo.top() - 1)
            self._frameGeometry = geo
            self.FrameGeometryUpdated.emit()

    @pyqtSlot(QObject, QUrl)
    def onCreated(self, obj, url):
        self._maskWin = obj


    @pyqtProperty(QRect, notify = FrameGeometryUpdated)
    def frameGeometry(self):
        return self._frameGeometry


class QmlResizable(QQuickView):
    ResizeStart = pyqtSignal()
    ResizeEnd = pyqtSignal()
    ResizeSizeUpdated = pyqtSignal(QRect)

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.setFlags(Qt.FramelessWindowHint)

        self._resizeEdge = ResizingEdges.Not
        self._resizePrevPos = None
        self._resizePrevGPos = None
        self._resizePrevGeo = None
        self._resizePaddings = None
        self._tentativeGeo = None

        self._isBeingDragged = False
        self._dragOffset = None
        self._dragHeight = None

        self._fakeFrame = FakeFrame(self)

    def setResizePaddings(self, top: int, right: int, bottom: int, left: int):
        self._resizePaddings = ResizingPaddings(top, right, bottom, left)

    def setDragHeight(self, height: int):
        self._dragHeight = height

    def mousePressEvent(self, qMouseEvent):
        if qMouseEvent.button() == Qt.LeftButton:
            pos = qMouseEvent.pos()
            self._resizeEdge = cursorInResizeRegion(pos, self.geometry(), self._resizePaddings)
            if self._resizeEdge:
                self._resizePrevPos = pos
                self._resizePrevGPos = qMouseEvent.globalPos()
                self._resizePrevGeo = self.geometry()
                self.ResizeStart.emit()
            else:
                inMoveZone = cursorInDragRegion(pos, self._dragHeight)
                if inMoveZone:
                    self._isBeingDragged = True
                    self._dragOffset = pos
                    self.setCursor(Qt.SizeAllCursor)

        super().mousePressEvent(qMouseEvent)

    def mouseMoveEvent(self, qMouseEvent):
        pos = qMouseEvent.pos()

        # set cursor
        resizeZone = cursorInResizeRegion(pos, self.geometry(), self._resizePaddings)

        if resizeZone:
            self.setCursor(edgeToCursor(resizeZone))

        if self._resizeEdge:
            geo = self.geometry()
            geo = resizeTo(resizeEdge = self._resizeEdge,
                           geo = geo,
                           pos = pos,
                           gPos = qMouseEvent.globalPos(),
                           prevGeo = self._resizePrevGeo,
                           prevPos = self._resizePrevPos,
                           minHeight = self.minimumHeight(),
                           minWidth = self.minimumWidth(),
                           maxHeight = self.maximumHeight(),
                           maxWidth = self.maximumWidth())
            self._tentativeGeo = geo
            self.ResizeSizeUpdated.emit(geo)
        else:
            inDragZone = cursorInDragRegion(pos, self._dragHeight)
            if self._isBeingDragged:
                newPos = qMouseEvent.globalPos() - self._dragOffset
                self.setPosition(newPos)
            else:
                if (not inDragZone) and (not resizeZone):
                    self.unsetCursor()

        super().mouseMoveEvent(qMouseEvent)

    @pyqtSlot()
    def _sendResizeEvent(self):
        e = QEvent(QEvent.Resize)
        QGuiApplication.postEvent(self, e)

    def mouseReleaseEvent(self, qMouseEvent):
        if qMouseEvent.button() == Qt.LeftButton:
            # resizing-related
            if self._resizeEdge:
                if self._tentativeGeo:
                    self.setGeometry(self._tentativeGeo)
                    self._tentativeGeo = None
                timer = QTimer.singleShot(50, self._sendResizeEvent)
                self._resizeEdge = ResizingEdges.Not
                self._resizePrevPos = None
                self._resizePrevGPos = None
                self._resizePrevGeo = None

                self.ResizeEnd.emit()
            if self._isBeingDragged:
                # drag-related
                self._isBeingDragged = False
                self._dragOffset = None
        super().mouseReleaseEvent(qMouseEvent)

    def show(self):
        if (not self._resizePaddings) or (not self._dragHeight):
            raise ValueError("You must setResizePaddings and setDragHeight first.")

        return super().show()
