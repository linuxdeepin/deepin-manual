"use strict";

angular.module("DManual")
    .factory("AdapterService", function Adapter($log, $rootScope) {
        let setMarkdown = function(path) {
            $log.log(`Markdown set to ${path}`);
            $rootScope.$broadcast("setMarkdown", path);
        };
        let result = {
            setMarkdown: setMarkdown,
        };
        window.adapter = result;
        return result;
    });

(function() {
    let AtomShell;
    try {
        AtomShell = require("remote");
    } catch (err) {
        AtomShell = null;
    }

    if (AtomShell) {
        let app = AtomShell.require("app");
        let mainWin = AtomShell.require("browser-window").fromId(1);

        window.maximize = function() {
            if (mainWin.isMaximized()) {
                mainWin.restore();
            } else {
                mainWin.maximize();
            }
        };
        window.minimize = mainWin.minimize;

        let body = document.getElementsByTagName("body")[0];
        mainWin.on("minimize", function() {

        });
        mainWin.on("maximize", function() {

        });

        let ipc = require("ipc");
        ipc.on("setMarkdown", function(path) {
            adapter.setMarkdown(path);
        });

        // Atom-shell automatically injects the `close` method.
        // window.close = function() {
        //
        // };
    } else {
        let stub = function () {
            console.log("You called a stub.");
        };
        window.maximize = window.minimize = window.close = stub;
        console.warn("No Shell Detected.");
    }
})();
