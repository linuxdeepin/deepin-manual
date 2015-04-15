import QtQuick 2.1
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
        anchors.fill: parent
        color: "transparent"
        border.color: borderColor
        border.width: 2
    }
}
