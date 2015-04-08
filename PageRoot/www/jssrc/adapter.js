"use strict";

angular.module("DManual")
    .factory("AdapterService", function Adapter($log, $rootScope, $window) {
        let setMarkdown = function(path) {
            $log.log(`Markdown set to ${path}`);
            $rootScope.$broadcast("setMarkdown", path);
        };
        let getShellType = function() {
            // try atom-shell
            try {
                require("remote");
                return "Atom-Shell";
            } catch (e) {}

            // assume oxide
            return "Oxide";
            return null;
        };
        let result = {
            setMarkdown: setMarkdown,
            getShellType: getShellType,
        };
        $window.adapter = result;
        return result;
    })
    .run(function($log, $injector, AdapterService, $window) {
        let shell = AdapterService.getShellType();
        let body = document.getElementsByTagName("body")[0];
        switch (shell) {
            case "Atom-Shell": {
                let AtomShell = require("remote");
                let app = AtomShell.require("app");
                let mainWin = AtomShell.require("browser-window").fromId(1);

                $window.maximize = function() {
                    if (mainWin.isMaximized()) {
                        mainWin.restore();
                    } else {
                        mainWin.maximize();
                    }
                };
                $window.minimize = mainWin.minimize;
                mainWin.on("maximize", function() {
                    angular.element(body).addClass("isMaximized");
                });
                mainWin.on("unmaximize", function() {
                    angular.element(body).removeClass("isMaximized");
                });
                // Atom-shell automatically injects the `close` method.
                // window.close = function() {
                //
                // };

                let ipc = require("ipc");
                ipc.on("setMarkdown", function(path) {
                    adapter.setMarkdown(path);
                });

                // Disallowing dropping
                let disallow = function(event) {
                    event.canDrop = false;
                    event.preventDefault();
                    return false;
                };
                angular.element(body).bind("dragstart", disallow);
                angular.element(body).bind("dragenter", disallow);
                angular.element(body).bind("dragover", disallow);
                angular.element(body).bind("dragend", disallow);
                angular.element(body).bind("drop", disallow);

                ipc.send("AdapterReady", true);
                break;
            }
            case "Oxide": {
                let sendMessage = function(id, args){
                    let newEvent = new CustomEvent("OxideSendMessage", {
                        detail: {
                            "msgId": id,
                            "args": args,
                        }
                    });
                    $window.dispatchEvent(newEvent);
                };

                let sendMessageNoReply = function(id, args){
                    let newEvent = new CustomEvent("OxideSendMessageNoReply", {
                        detail: {
                            "msgId": id,
                            "args": args,
                        }
                    });
                    $window.dispatchEvent(newEvent);
                };
                let connectMessage = function(signalName, handler){
                    signalName = signalName.slice(0, 1).toUpperCase() +
                                 signalName.slice(1);
                    let oxideSignalName = "OxideSignal" + signalName;
                    $window.addEventListener(oxideSignalName, function(e){
                        handler(e.detail);
                    });
                };
                let disconnectMessage = function(signalName){
                    let oxideSignalName = "OxideSignal" + signalName;
                    $window.removeEventListener(oxideSignalName);
                };

                let oxideWrap = {
                    "sendMessage": sendMessage,
                    "sendMessageNoReply": sendMessageNoReply,
                    "connect": connectMessage,
                    "disconnect": disconnectMessage,
                };
                $window.oxide = oxideWrap;
                $window.maximize = function() {
                    oxideWrap.sendMessageNoReply("JSMESSAGE", "maximize");
                };
                $window.minimize = function() {
                    oxideWrap.sendMessageNoReply("JSMESSAGE", "minimize");
                };
                $window.close = function() {
                    oxideWrap.sendMessageNoReply("JSMESSAGE", "close");
                };
                document.addEventListener("OxideSignalMessage", function(event) {
                    let payload = event.detail;
                    switch (payload.type) {
                        case "FrameControl": {
                            switch (payload.msg) {
                                case "maximized": {
                                    angular.element(body).addClass("isMaximized");
                                    break;
                                }
                                case "unmaximized": {
                                    angular.element(body).removeClass("isMaximized");
                                    break;
                                }
                            }
                            break;
                        }
                        case "SetMarkdown": {
                            AdapterService.setMarkdown(payload.msg);
                            break;
                        }
                        default: {
                            console.warn(`Unhandled msg from QML: ${payload}`);
                        }
                    }
                });
                setTimeout(function() {
                    oxideWrap.sendMessageNoReply("JSMESSAGE", "adapter_ready");
                }, 0);
                break;
            }
            default: {
                let stub = function () {
                    console.log("You called a stub.");
                };
                $window.maximize = $window.minimize = $window.close = stub;
                console.warn("No Shell Detected.");
                break;
            }
        }
    });
