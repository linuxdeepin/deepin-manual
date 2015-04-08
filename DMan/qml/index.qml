import QtQuick 2.2
import QtQuick.Controls 1.2
import com.canonical.Oxide 1.0

Rectangle {
    width: 946
    height: 600
    property string oxideContext: "messaging://"
    property string usMsgId: "JSMESSAGE"

    property var starturl: Qt.resolvedUrl("../../PageRoot/www/index.html")

    WebView {
        id: webView
        anchors.fill: parent
        url: starturl
        focus: true
        context: webContext
        preferences.allowUniversalAccessFromFileUrls: true
        preferences.allowFileAccessFromFileUrls: true
        Component.onCompleted: {
            DManBridge.windowStateChanged.connect(function(winState) {
                if (winState && Qt.WindowMaximized) {
                    rootFrame.sendMessage(oxideContext, usMsgId, {
                        detail: "maximized",
                    })
                } else {
                    rootFrame.sendMessage(oxideContext, usMsgId, {
                        detail: "unmaximized",
                    })
                }
            })
            rootFrame.addMessageHandler(jsMessageHandler)
        }
    }

    ScriptMessageHandler {
        id: jsMessageHandler
        msgId: usMsgId
        contexts: oxideContext
        callback: function (content) {
            var msg = content.args.detail
            switch (msg) {
                case "maximize": {
                    DManBridge.signalMaximize()
                    break;
                }
                case "minimize": {
                    DManBridge.signalMinimize()
                    break;
                }
                case "close": {
                    DManBridge.signalClose()
                    break;
                }
                default: {
                    console.warn("Unknown msg: " + msg)
                }
            }
        }
    }

    WebContext {
        id: webContext
        cachePath: "file:///tmp/"
        dataPath: "file:///tmp/"
        devtoolsEnabled: true
        devtoolsPort: 18080
        userScripts: [
            UserScript {
                context: oxideContext
                matchAllFrames: true
                url: Qt.resolvedUrl("../../PageRoot/www/jssrc/adapter_oxide.js")
            }
        ]
    }
    Component.onCompleted: {

    }
}
