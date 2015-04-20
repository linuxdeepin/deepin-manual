"use strict";

angular.module("DManual")
    .factory("AdapterService", function Adapter($log, $rootScope, $window) {
        let setMarkdown = function(markdownDir) {
            $log.log(`Markdown set to ${markdownDir}`);
            $rootScope.$broadcast("setMarkdown", markdownDir);
        };
        let getShellType = function() {
            // try DAE
            if ($window.DAE) {
                return "DAE";
            }

            // try atom-shell
            try {
                let req = $window.require;
                req("remote");
                return "Atom-Shell";
            } catch (e) {}

            // assume oxide
            return "Oxide";
            return null;
        };
        let setDebugMode = function(on) {
            on = !!on;
            $log.log(`Debug mode ${on}`);
            $rootScope.isDebugging = on;
            let body = document.body;
            if (on) {
                body.classList.add("debug-mode");
            } else {
                body.classList.remove("debug-mode");
            }
        };
        let showTooltip = function(text, rect) {
            let x = rect.left;
            let y = rect.top + rect.height / 2;

            switch (getShellType()) {
                case "Oxide": {
                    if ($window.oxide) {
                        $window.oxide.sendMessageNoReply("JSMESSAGE", {
                            type: "ShowTooltip",
                            body: {
                                text: text,
                                x: x,
                                y: y,
                            },
                        });
                    } else {
                        $log.warn("Oxide object doesn't exist, so showTooltip will be noop");
                    }
                    break;
                }
                default: {
                    $log.warn("No shell support for showTooltip");
                }
            }
        };
        let setTitle = function(title) {
            switch (getShellType()) {
                case "Oxide": {
                    if ($window.oxide) {
                        $window.oxide.sendMessageNoReply("JSMESSAGE", {
                            type: "SetTitle",
                            body: title,
                        });
                    } else {
                        $log.warn("Oxide object doesn't exist, so setTitle will be noop");
                    }
                    break;
                }
                default: {
                    $log.warn("No shell support for setTitle");
                }
            }
        };
        let openExternalBrowser = function() {
            switch (getShellType()) {
                case "Oxide": {
                    if ($window.oxide) {
                        $window.oxide.sendMessageNoReply("JSMESSAGE", {
                            type: "OpenExternalBrowser",
                            body: "",
                        });
                    } else {
                        $log.warn("Oxide object doesn't exist, so openExternalBrowser will be noop");
                    }
                    break;
                }
                default: {
                    $log.warn("No shell support for openExternalBrowser");
                }
            }
        };
        let result = {
            setMarkdown: setMarkdown,
            getShellType: getShellType,
            setDebugMode: setDebugMode,
            showTooltip: showTooltip,
            setTitle: setTitle,
            openExternalBrowser: openExternalBrowser,
        };
        $window.adapter = result;
        return result;
    })
    .run(function($log, $injector, AdapterService, $window, $interval, localeService) {
        let shell = AdapterService.getShellType();
        let body = document.body;
        switch (shell) {
            case "Atom-Shell": {
                let req = $window.require;
                let AtomShell = req("remote");
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

                let ipc = req("ipc");
                ipc.on("setMarkdown", function(msg) {
                    adapter.setMarkdown(msg.dmanDir);
                    localeService.setLocale(msg.uiLangs);
                });
                ipc.on("Debug", function(on) {
                    adapter.setDebugMode(on);
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
                let userScriptSyncTimer = null;
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
                    oxideWrap.sendMessageNoReply("JSMESSAGE", {
                        type: "FrameControl",
                        body: "maximize",
                    });
                };
                $window.minimize = function() {
                    oxideWrap.sendMessageNoReply("JSMESSAGE", {
                        type: "FrameControl",
                        body: "minimize",
                    });
                };
                $window.close = function() {
                    oxideWrap.sendMessageNoReply("JSMESSAGE", {
                        type: "FrameControl",
                        body: "close",
                    });
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
                            AdapterService.setMarkdown(payload.msg.dmanDir);
                            localeService.setLocale(payload.msg.uiLangs);
                            break;
                        }
                        case "Debug": {
                            AdapterService.setDebugMode(payload.msg);
                            break;
                        }
                        case "AdapterStatus": {
                            if (payload.msg === "pong") {
                                oxideWrap.sendMessageNoReply("JSMESSAGE", {
                                    type: "AdapterStatus",
                                    body: "adapter_ready",
                                });
                                $interval.cancel(userScriptSyncTimer);
                            }
                            break;
                        }
                        default: {
                            console.warn(`Unhandled msg from QML: ${payload}`);
                        }
                    }
                });
                userScriptSyncTimer = $interval(function() {
                    oxideWrap.sendMessageNoReply("JSMESSAGE", {
                        type: "AdapterStatus",
                        body: "ping",
                    });
                }, 20);
                break;
            }
            case "DAE": {
                let app = $window.DAE.app;
                app.setMinSize(946, 600);
                app.setResizerSize(5);
                app.setFrameless(true);
                app.setTransBackground(true);
                app.show();

                app.addEvent({
                    statechange: function() {
                        let body = document.body;
                        if (app.isMaximized()) {
                            angular.element(body).addClass("isMaximized");
                        } else {
                            angular.element(body).removeClass("isMaximized");
                        }
                    }
                });
                $window.maximize = function() {
                    if (app.isMaximized()) {
                        app.normalize();
                    } else {
                        app.maximize();
                    }
                };
                $window.minimize = app.minimize;
                $window.close = function() {
                    app.setClosable(true);
                    app.close();
                };
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
