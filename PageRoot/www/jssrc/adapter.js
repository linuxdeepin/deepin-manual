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

            return null;
        };
        let result = {
            setMarkdown: setMarkdown,
            getShellType: getShellType,
        };
        $window.adapter = result;
        return result;
    })
    .run(function($log, $injector, AdapterService) {
        let shell = AdapterService.getShellType();
        switch (shell) {
            case "Atom-Shell": {
                let AtomShell = require("remote");
                let app = AtomShell.require("app");
                let body = document.getElementsByTagName("body")[0];
                let mainWin = AtomShell.require("browser-window").fromId(1);

                window.maximize = function() {
                    if (mainWin.isMaximized()) {
                        mainWin.restore();
                    } else {
                        mainWin.maximize();
                    }
                };
                window.minimize = mainWin.minimize;
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
            default: {
                let stub = function () {
                    console.log("You called a stub.");
                };
                window.maximize = window.minimize = window.close = stub;
                console.warn("No Shell Detected.");
                break;
            }
        }
    });
