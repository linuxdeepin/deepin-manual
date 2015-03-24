import QtQuick 2.2
import QtQuick.Controls 1.2
import com.canonical.Oxide 1.0

Rectangle {
    width: 1000
    height: 300

    property var starturl: Qt.resolvedUrl("../../PageRoot/www/index.html")

    function updateWebView() {
        webView.url = locationField.text
    }

    Column {
        anchors.fill: parent
        Row {
            id: navRow
            width: parent.width
            height: locationField.height
            Button  {
                id: backButton
                height: locationField.height
                text: "后退"

                onClicked: {
                    webView.goBack()
                }
            }

            Button  {
                id: forwardButton
                height: locationField.height
                text: "前进"

                onClicked: {
                    webView.goForward()
                }
            }

            TextField {
                id: locationField
                width: parent.width - backButton.width - forwardButton.width
                text: starturl

                onAccepted: {
                    updateWebView()
                }
            }
        }

        WebView {
            id: webView
            width: parent.width
            height: parent.height - navRow.height - statusLabel.height
            url: starturl
            focus: true
            context: webcontext
            preferences.allowUniversalAccessFromFileUrls: true
            preferences.allowFileAccessFromFileUrls: true
            onUrlChanged: {
                locationField.text = url
            }
        }

        WebContext {
            id: webcontext
            cachePath: "file:///tmp/"
            dataPath: "file:///tmp/"
            devtoolsEnabled: true
            devtoolsPort: 18080
        }

        Label {
            id: statusLabel
            text: webView.loading ?
                    "Loading" + " (%1%)".arg(webView.loadProgress) :
                    "Page loaded"
            width: parent.width
        }

    }
}
