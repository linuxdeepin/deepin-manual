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
                        detail: {
                            type: "FrameControl",
                            msg: "maximized",
                        },
                    })
                } else {
                    rootFrame.sendMessage(oxideContext, usMsgId, {
                        detail: {
                            type: "FrameControl",
                            msg: "unmaximized",
                        },
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
                case "adapter_ready": {
                    console.log("Adapter Ready");
                    webView.rootFrame.sendMessage(oxideContext, usMsgId, {
                        detail: {
                            type: "SetMarkdown",
                            msg: DManBridge.mdUrl,
                        },
                    })
                    webView.rootFrame.sendMessage(oxideContext, usMsgId, {
                        detail: {
                            type: "Debug",
                            msg: DManBridge.debugMode,
                        },
                    })
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
                Component.onCompleted: {
                    console.log("UserScript Ready");
                }
            }
        ]
    }
}
