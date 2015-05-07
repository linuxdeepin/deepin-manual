import QtQuick 2.2
import QtQuick.Window 2.1
import QtGraphicalEffects 1.0

Window {
	id: win
	flags: Qt.FramelessWindowHint | Qt.Popup
    width: Screen.width
    height: Screen.height
	visible: false
	color: "transparent"
    
    property string borderColor: "#AAAEC1D5"

    Rectangle {
        color: "transparent"
        border.color: borderColor
        border.width: 2
        x: frameView.frameGeometry.x
        y: frameView.frameGeometry.y
        width: frameView.frameGeometry.width
        height: frameView.frameGeometry.height
    }
}
