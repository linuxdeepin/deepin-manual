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

from PyQt5.QtCore import QObject, QByteArray, QBuffer, pyqtSlot, pyqtSignal, QSize, QPoint
from PyQt5.Qt import Qt, QApplication, QDialog, QEvent, QPalette, QPainter, QImage, QIcon
from PyQt5.QtWidgets import QShortcut, qApp
from PyQt5.QtGui import QCursor

import dae.assets as assets
from dae.api import API
from dae.webview import WebView
from dae.shadowwindow import ShadowWindow
import dae.xutils as xutils

LEVEL0, LEVEL1, LEVEL2, LEVEL3, LEVEL4 = 0, 1, 2, 3, 4

class Window(ShadowWindow):

    Move = 0b0000
    ResizeTop = 0b0001
    ResizeRight = 0b0010
    ResizeBottom = 0b0100
    ResizeLeft = 0b1000

    onshow = pyqtSignal()
    onhide = pyqtSignal()
    onfocus = pyqtSignal()
    onblur = pyqtSignal()
    onclose = pyqtSignal()
    onmove = pyqtSignal(int, int, int, int)
    onresize = pyqtSignal(int, int, int, int)
    onstatechange = pyqtSignal()
    onmouseenter = pyqtSignal()
    onmouseleave = pyqtSignal()

    windowLevel = LEVEL0


    def __init__(self, parent = None, url = '', width = None, height = None, isDialog = False):
        super(Window, self).__init__(parent if isDialog else None)

        self.assets = assets
        assets.windows.append(self)
        if width is None:
            width = assets.manifest['width']
        if height is None:
            height = assets.manifest['height']
        windowFlags = Qt.WindowTitleHint | \
            Qt.WindowSystemMenuHint | \
            Qt.WindowMinimizeButtonHint | \
            Qt.WindowMaximizeButtonHint | \
            Qt.WindowCloseButtonHint
        if isDialog:
            windowFlags |= Qt.Dialog
        else:
            windowFlags |= Qt.CustomizeWindowHint
        self.dragParams = {'type': 0, 'x': self.shadowWidth, 'y': self.shadowWidth, 'size': 5, 'draging': False}
        self.api = API(self)
        self.parent = parent
        self.resize(width, height)
        self.setMouseTracking(True)
        self.setWindowFlags(windowFlags)
        self.setAttribute(Qt.WA_QuitOnClose, True)
        self.setAttribute(Qt.WA_DeleteOnClose, True)
        self.setAttribute(Qt.WA_TranslucentBackground, True)

        self.setWindowTitle(assets.manifest['name'])
        self.setResizable(assets.manifest['resizable'])
        self.setFrameless(assets.manifest['frameless'])
        self.setWindowIcon(QIcon(assets.manifest['icon']))

        self._win_id = self.winId().__int__()

        self.webView = WebView(self, url)
        self.verticalLayout.addWidget(self.webView)

        if assets.manifest['debug']:
            shortcut = QShortcut(self)
            shortcut.setKey(Qt.Key_F12)
            shortcut.activated.connect(self.showInspector)

        self._old_window_state = self.windowState()
        self._is_max_to_min = False
        self._maximizable = True

        self.setClosable(True)

    def getXWindow(self):
        return xutils.get_xwindow(int(self.winId()))

    def hasWindowFlags(self, windowFlags):
        return int(self.windowFlags() & windowFlags) != 0

    def addWindowFlags(self, windowFlags):
        if not self.hasWindowFlags(windowFlags):
            flags = self.windowFlags()
            flags |= windowFlags
            self.setWindowFlags(flags)

    def removeWindowFlags(self, windowFlags):
        if self.hasWindowFlags(windowFlags):
            flags = self.windowFlags()
            flags = flags &~ windowFlags
            self.setWindowFlags(flags)

    def getCursorType(self, x, y):
        width = self.width()
        height = self.height()
        size = self.dragParams['size']
        margin = self.shadowWidth
        isResizableX = self.isResizableX()
        isResizableY = self.isResizableY()
        cursorType = Window.Move
        if x >= margin and x <= width - margin and y >= margin and y <= height - margin:
            if y <= size + margin and isResizableY:
                cursorType |= Window.ResizeTop
            elif y >= height - margin - size and isResizableY:
                cursorType |= Window.ResizeBottom
            if x <= size + margin and isResizableX:
                cursorType |= Window.ResizeLeft
            elif x >= width - margin - size and isResizableX:
                cursorType |= Window.ResizeRight
        return cursorType

    # Slots

    # 获取父窗口
    @pyqtSlot(result = QObject)
    def opener(self):
        return self.parent

    # 任务栏闪烁
    @pyqtSlot()
    @pyqtSlot(int)
    def alert(self, msec = 0):
        QApplication.alert(self, msec)

    # 执行简单的JS
    @pyqtSlot(str, result = QObject)
    def eval(self, javaScript):
        obj = QObject(self)
        obj.setObjectName('evalResult')
        obj.setProperty('result', self.webView.eval(javaScript))
        return obj

    # 截图
    @pyqtSlot(result = str)
    @pyqtSlot(bool, result = str)
    @pyqtSlot(bool, str, result = bool)
    def capture(self, fullScreen = False, filename = ''):
        if fullScreen:
            image = QApplication.primaryScreen().grabWindow(0)
        else:
            image = QImage(self.webView.mainFrame.contentsSize(), QImage.Format_ARGB32)
            painter = QPainter(image)
            self.webView.mainFrame.render(painter)
            painter.end()
        if filename:
            return image.save(filename)
        else:
            data = QByteArray()
            buffer = QBuffer(data)
            buffer.open(QBuffer.WriteOnly)
            image.save(buffer, 'PNG')
            return bytes(data.toBase64()).decode()

    # 打开开发者工具
    @pyqtSlot()
    def showInspector(self):
        self.webView.page().showInspector()

    # 关闭开发者工具
    @pyqtSlot()
    def hideInspector(self):
        self.webView.page().hideInspector()

    # 搜索文本
    @pyqtSlot(str, result = bool)
    def findText(self, text):
        return self.webView.page().findText(text)

    # 设置窗口图标
    @pyqtSlot(str)
    def setIcon(self, icon):
        if isinstance(icon, str):
            icon = QIcon(self.api.normUrl(icon))
        self.setWindowIcon(icon)

    # 设置内容是否已被修改（标题中需要有[*]标志）
    @pyqtSlot(bool)
    def setModified(self, modified):
        self.setWindowModified(modified)

    # 获取内容是否已被修改
    @pyqtSlot(result = bool)
    def isModified(self):
        return self.isWindowModified()

    # 设置是否模态
    @pyqtSlot(bool)
    def setModal(self, modal):
        super(Window, self).setModal(modal)

    # 获取是否模态
    @pyqtSlot(result = bool)
    def isModal(self):
        return super(Window, self).isModal()

    # 设置是否在任务栏中显示
    @pyqtSlot(bool)
    def setShowInTaskbar(self, showInTaskbar):
        if showInTaskbar:
            self.setWindowFlags(self.windowFlags() & ~ Qt.Tool)
        else:
            self.setWindowFlags(self.windowFlags() | Qt.Tool)

    # 获取是否在任务栏中显示
    @pyqtSlot(result = bool)
    def isShowInTaskbar(self):
        return self.hasWindowFlags(Qt.Tool)

    # 设置窗口是否置顶
    @pyqtSlot(bool)
    def setStaysOnTop(self, staysOnTop):
        if staysOnTop:
            xutils.do_stay_on_top(self.getXWindow())
        else:
            xutils.undo_stay_on_top(self.getXWindow())

    # 获取窗口是否置顶
    @pyqtSlot(result = bool)
    def isStaysOnTop(self):
        return xutils.is_stay_on_top(self.getXWindow())

    # 设置是否显示系统边框
    @pyqtSlot(bool)
    def setFrameless(self, frameless):
        if frameless:
            self.setWindowFlags(Qt.FramelessWindowHint)
        else:
            self.addWindowFlags(Qt.WindowMaximizeButtonHint)
            self.addWindowFlags(Qt.WindowMinMaxButtonsHint)
            self.addWindowFlags(Qt.WindowSystemMenuHint)
            self.removeWindowFlags(Qt.FramelessWindowHint)

    # 获取是否显示系统边框
    @pyqtSlot(result = bool)
    def isFrameless(self):
        return self.hasWindowFlags(Qt.FramelessWindowHint)

    # 设置是否背景透明
    @pyqtSlot(bool)
    def setTransBackground(self, transBackground):
        palette = self.palette()
        if transBackground:
            palette.setBrush(QPalette.Base, Qt.transparent)
        else:
            palette.setBrush(QPalette.Base, Qt.white)
        self.setPalette(palette)

    # 获取是否背景透明
    @pyqtSlot(result = bool)
    def isTransBackground(self):
        palette = self.palette()
        return palette.brush(QPalette.Base) == Qt.transparent

    # 设置是否鼠标事件穿透
    @pyqtSlot(bool)
    def setTransMouseEvent(self, transMouseEvent):
        self.setAttribute(Qt.WA_TransparentForMouseEvents, transMouseEvent)
        self.setWindowFlags(self.windowFlags() | Qt.WindowTitleHint)

    # 获取是否是否鼠标事件穿透
    @pyqtSlot(result = bool)
    def isTransMouseEvent(self):
        return self.testAttribute(Qt.WA_TransparentForMouseEvents)

    # 设置窗口不透明度
    @pyqtSlot(float)
    def setOpacity(self, opacity):
        self.setWindowOpacity(opacity)

    # 获取窗口不透明度
    @pyqtSlot(result = float)
    def getOpacity(self):
        return self.windowOpacity()

    # 设置尺寸是否可调整
    @pyqtSlot(bool)
    def setResizable(self, resizable):
        if resizable:
            self.setMinimumSize(5, 5)
            self.setMaximumSize(0xFFFFFF, 0xFFFFFF)
        else:
            self.setFixedSize(self.width(), self.height())

    # 获取尺寸是否可调整
    @pyqtSlot(result = bool)
    def isResizable(self):
        return self.minimumSize() != self.maximumSize()

    # 设置宽度是否可调整
    @pyqtSlot(bool)
    def setResizableX(self, resizableX):
        if resizableX:
            self.setMinimumWidth(5)
            self.setMaximumWidth(0xFFFFFF)
        else:
            self.setFixedWidth(self.width())

    # 获取宽度是否可调整
    @pyqtSlot(result = bool)
    def isResizableX(self):
        return self.minimumWidth() != self.maximumWidth()

    # 设置高度是否可调整
    @pyqtSlot(bool)
    def setResizableY(self, resizableY):
        if resizableY:
            self.setMinimumHeight(5)
            self.setMaximumHeight(0xFFFFFF)
        else:
            self.setFixedHeight(self.height())

    # 获取高度是否可调整
    @pyqtSlot(result = bool)
    def isResizableY(self):
        return self.minimumHeight() != self.maximumHeight()

    # 设置是否可最小化
    @pyqtSlot(bool)
    def setMinimizable(self, minimizable):
        if minimizable:
            self.setWindowFlags(self.windowFlags() | Qt.WindowMinimizeButtonHint)
        else:
            self.setWindowFlags(self.windowFlags() & ~ Qt.WindowMinimizeButtonHint)

    # 获取是否可最小化
    @pyqtSlot(result = bool)
    def isMinimizable(self):
        return True

    # 设置是否可最大化
    @pyqtSlot(bool)
    def setMaximizable(self, maximizable):
        if maximizable:
            self.setWindowFlags(self.windowFlags() | Qt.WindowMaximizeButtonHint)
        else:
            self.setWindowFlags(self.windowFlags() & ~ Qt.WindowMaximizeButtonHint)

    # 获取是否可最大化
    @pyqtSlot(result = bool)
    def isMaximizable(self):
        return self._maximizable

    # 设置是否可关闭
    @pyqtSlot(bool)
    def setClosable(self, closable):
        if closable:
            self.setWindowFlags(self.windowFlags() | Qt.WindowCloseButtonHint)
        else:
            self.setWindowFlags(self.windowFlags() & ~ Qt.WindowCloseButtonHint)

    # 获取是否可关闭
    @pyqtSlot(result = bool)
    def isClosable(self):
        return self.hasWindowFlags(Qt.WindowCloseButtonHint)

    # 获取是否已最小化
    @pyqtSlot(result = bool)
    def isMinimized(self):
        return super(Window, self).isMinimized()

    # 获取是否已最大化
    @pyqtSlot(result = bool)
    def isMaximized(self):
        return super(Window, self).isMaximized()

    # 获取是否已最大化
    @pyqtSlot(result = bool)
    def isFullScreen(self):
        return super(Window, self).isFullScreen()

    # 最小化
    @pyqtSlot()
    def minimize(self):
        if self.isMinimizable():
            self.showMinimized()

    # 还原
    @pyqtSlot()
    def normalize(self):
        self.showNormal()

    # 最大化
    @pyqtSlot()
    def maximize(self):
        if self.isMaximizable():
            self.showMaximized()

    # 全屏
    @pyqtSlot()
    def showFullScreen(self):
        super(Window, self).showFullScreen()

    # 设置窗口坐标
    @pyqtSlot(int, int)
    def setPos(self, x, y):
        self.move(x, y)

    # 设置窗口尺寸
    @pyqtSlot(int, int)
    def setSize(self, width, height):
        self.resize(width, height)

    # 设置窗口最小尺寸
    @pyqtSlot(int, int)
    def setMinSize(self, width, height):
        self.setMinimumSize(width, height)

    # 设置窗口最大尺寸
    @pyqtSlot(int, int)
    def setMaxSize(self, width, height):
        self.setMaximumSize(width, height)

    # 设置尺寸控制大小
    @pyqtSlot(int)
    def setResizerSize(self, size):
        self.dragParams['size'] = size

    # 获取尺寸控制大小
    @pyqtSlot(result = int)
    def getResizerSize(self):
        return self.dragParams['size']

    # 是否正在拖动
    @pyqtSlot(result = bool)
    def isDraging(self):
        return self.dragParams['draging']

    # 开始拖动
    @pyqtSlot()
    def dragStart(self):
        self.dragParams['draging'] = True

    # 结束拖动
    @pyqtSlot()
    def dragStop(self):
        self.dragParams['type'] = Window.Move
        self.dragParams['draging'] = False

    # 窗口获取焦点
    @pyqtSlot()
    def activate(self):
        self.activateWindow()

    # 窗口是否已获得焦点
    @pyqtSlot(result = bool)
    def isActive(self):
        return self.isActiveWindow()

    # 设置窗口可用状态
    @pyqtSlot(bool)
    def setEnabled(self, enabled):
        super(Window, self).setEnabled(enabled)

    # 窗口是否可用
    @pyqtSlot(result = bool)
    def isEnabled(self):
        return super(Window, self).isEnabled()

    # 设置窗口可见状态
    @pyqtSlot(result = bool)
    def setVisible(self, visible):
        super(Window, self).setVisible(visible)

    # 窗口是否显示
    @pyqtSlot(result = bool)
    def isVisible(self):
        return super(Window, self).isVisible()

    # 关闭窗口
    @pyqtSlot()
    def close(self):
        super(Window, self).close()

    @pyqtSlot()
    def quit(self):
        for win in assets.windows:
            win.close()
        qApp.quit()

    # Events
    def keyPressEvent(self, event):
        if(event.key() == Qt.Key_Escape):
            event.ignore()

    # 被webview调用
    def mousePressEvent(self, event):
        self.dragParams['x'] = event.x()
        self.dragParams['y'] = event.y()
        self.dragParams['globalX'] = event.globalX()
        self.dragParams['globalY'] = event.globalY()
        self.dragParams['width'] = self.width()
        self.dragParams['height'] = self.height()
        if self.dragParams['type'] != Window.Move \
            and self.isFrameless() \
            and not self.isMaximized() \
            and not self.isFullScreen():
                self.dragStart()

    def mouseReleaseEvent(self, event):
        self.dragStop()

    def mouseMoveEvent(self, event):
        if self.isFrameless() and not self.isMaximized() and not self.isFullScreen():
            # 判断鼠标类型
            cursorType = self.dragParams['type']
            if not self.dragParams['draging']:
                cursorType = self.dragParams['type'] = self.getCursorType(event.x(), event.y())

            # 设置鼠标形状
            if cursorType in (Window.ResizeTop, Window.ResizeBottom):
                self.webView.setCursor(Qt.SizeVerCursor)
            elif cursorType in (Window.ResizeLeft, Window.ResizeRight):
                self.webView.setCursor(Qt.SizeHorCursor)
            elif cursorType in (Window.ResizeTop | Window.ResizeRight, Window.ResizeLeft | Window.ResizeBottom):
                self.webView.setCursor(Qt.SizeBDiagCursor)
            elif cursorType in (Window.ResizeTop | Window.ResizeLeft, Window.ResizeRight | Window.ResizeBottom):
                self.webView.setCursor(Qt.SizeFDiagCursor)
            elif self.dragParams['draging']:
                self.webView.setCursor(Qt.ArrowCursor)

        # 判断窗口拖动
        dragType = self.dragParams['type']
        if self.dragParams['draging'] and not self.isMaximized() and not self.isFullScreen():
            x = self.x()
            y = self.y()
            width = self.width()
            height = self.height()
            if dragType == Window.Move:
                x = event.globalX() - self.dragParams['x']
                y = event.globalY() - self.dragParams['y']
            elif self.isFrameless():
                if dragType & Window.ResizeTop == Window.ResizeTop:
                    y = event.globalY() - self.shadowWidth
                    height = self.dragParams['height'] + self.dragParams['globalY'] - event.globalY()
                elif dragType & Window.ResizeBottom == Window.ResizeBottom:
                    height = self.dragParams['height'] - self.dragParams['globalY'] + event.globalY()
                if dragType & Window.ResizeLeft == Window.ResizeLeft:
                    x = event.globalX() - self.shadowWidth
                    width = self.dragParams['width'] + self.dragParams['globalX'] - event.globalX()
                elif dragType & Window.ResizeRight == Window.ResizeRight:
                    width = self.dragParams['width'] - self.dragParams['globalX'] + event.globalX()
            else:
                return

            if width < self.minimumWidth():
                width = self.minimumWidth()
            elif width > self.maximumWidth():
                width = self.maximumWidth()
            if height < self.minimumHeight():
                height = self.minimumHeight()
            elif height > self.maximumHeight():
                height = self.maximumHeight()

            xutils.set_geometry(self.getXWindow(), x, y, width, height)

    def changeEvent(self, event):
        if event.type() == QEvent.ActivationChange:
            if self.isActiveWindow():
                self.onfocus.emit()
            else:
                self.onblur.emit()
        elif event.type() == QEvent.WindowStateChange:
            new_window_state = self.windowState()
            if new_window_state == Qt.WindowMaximized or \
                    new_window_state == Qt.WindowFullScreen:
                self.setShadowWidth(0)
            else:
                self.setShadowWidth(10)
            self.onstatechange.emit()

            self._old_window_state = new_window_state

    def resizeEvent(self, event):
        self.onresize.emit(
                event.oldSize().width(), event.oldSize().height(),
                event.size().width(), event.size().height()
                )

        # workround: it can't be set Antialiasing in setRoundedCorners method
        # use `self.radius - 1` to workround the problem
        if self.radius > 1:
            self.webView.setRoundedCorners(self.webView.rect(), self.radius - 1)

    def moveEvent(self, event):
        pos = event.pos()
        oldPos = event.oldPos()
        self.onmove.emit(pos.x(), pos.y(), oldPos.x(), oldPos.y())

    def enterEvent(self, event):
        self.onmouseenter.emit()

    def leaveEvent(self, event):
        self.onmouseleave.emit()

    def showEvent(self, event):
        self.onshow.emit()

    def hideEvent(self, event):
        self.onhide.emit()

    def closeEvent(self, event):
        self.onclose.emit()
        if self.isClosable():
            self.webView.close()
            assets.windows = [i for i in assets.windows if i != self]
            event.accept()
        else:
            event.ignore()

    def event(self, event):
        if event.type() == QEvent.WindowStateChange:
            if int(self.windowState()) == 3 and \
               event.oldState() == Qt.WindowMaximized:
               self.windowLevel = LEVEL1
            # 3 -> minimized
            # minimized -> normal
            elif self.windowLevel == LEVEL1:
               self.windowLevel = LEVEL2
            # normal -> minimized
            elif self.windowLevel == LEVEL2:
               self.windowLevel = LEVEL3
            # minimized -> normal -> maximized
            elif self.windowLevel == LEVEL3:
               self.maximize()
               self.windowLevel = LEVEL0
        return super(Window, self).event(event)

    @pyqtSlot()
    def showCenter(self):
        screen = qApp.primaryScreen()
        geometry = screen.availableGeometry()
        x = geometry.x() + (geometry.width() - self.width())/2
        y = geometry.y() + (geometry.height() - self.height())/2
        self.move(x, y)
        self.show()

    @pyqtSlot()
    def showCurrentScreenCenter(self):
        screen = qApp.desktop()
        geometry = screen.screenGeometry(screen.screenNumber(QCursor.pos()))
        x = geometry.x() + (geometry.width() - self.width())/2
        y = geometry.y() + (geometry.height() - self.height())/2
        self.move(x, y)
        self.show()
