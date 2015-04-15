import QtQuick 2.2
import QtQuick.Controls 1.2
import com.canonical.Oxide 1.0

Rectangle {
    id: rect
    width: 946
    height: 600
    property string oxideContext: "messaging://"
    property string usMsgId: "JSMESSAGE"

    property var starturl: Qt.resolvedUrl("../../PageRoot/www/index.html")

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

    function showTooltip(toolTip, x, y)
    {
        var width = getStringPixelSize(toolTip, 13) + 28
        x -= width
        y -= 12 // half the length of the height of the tooltip
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
            var payload = content.args.detail
            switch (payload.type) {
                case "FrameControl": {
                    switch (payload.body) {
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
                    }
                    break;
                }
                case "AdapterStatus": {
                    switch (payload.body) {
                        case "ping": {
                            webView.rootFrame.sendMessage(oxideContext, usMsgId, {
                                detail: {
                                    type: "AdapterStatus",
                                    msg: "pong",
                                },
                            })
                            break
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
                            console.warn("Didn't handle AdapterStatus, " + payload.body)
                         }
                    }
                }
                case "SetTitle": {
                    DManBridge.setTitle(payload.body)
                    break
                }
                case "ShowTooltip": {
                    if (payload.body.text) {
                        var tooltip = payload.body.text
                        var geo = DManBridge.currentGeometry()
                        var x = payload.body.x + geo.x
                        var y = payload.body.y + geo.y
                        showTooltip(tooltip, x, y)
                    } else {
                        closeTooltip()
                    }
                    break
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
