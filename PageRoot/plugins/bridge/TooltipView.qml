import QtQuick 2.2
import Deepin.Widgets 1.0

Item {
    property var calculateTextComponent : Component {
        Text {
            visible: false
        }
    }

    function getStringPixelSize(preText,fontPixelSize) {
        var tmpText = calculateTextComponent.createObject(null, { "text": preText , "font.pixelSize": fontPixelSize})
        var width = tmpText.width
        return width
    }

    property var singleLineTipComponent: Qt.createComponent("SingleLineTip.qml");
    property var singleLineTipPage;

    function showTooltip(toolTip, x, y) {
        var width = getStringPixelSize(toolTip, 13) + 28
        x -= width
        if (!singleLineTipPage)
        {
            singleLineTipPage = singleLineTipComponent.createObject(undefined)
            singleLineTipPage.x = x
            singleLineTipPage.y = y
            singleLineTipPage.toolTip = toolTip
            singleLineTipPage.width = width
            singleLineTipPage.destroyInterval = -1
            singleLineTipPage.showTipAtRight()
        }
    }

    function closeTooltip()
    {
        if (singleLineTipPage) {
            singleLineTipPage.destroyTip()
            singleLineTipPage = null
        }
    }
    Component.onCompleted: {
        view.sigShowTooltip.connect(function(text, x, y) {
            if (text) {
                showTooltip(text, x, y)
            } else {
                closeTooltip()
            }
        })
    }
}