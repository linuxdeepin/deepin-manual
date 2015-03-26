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
    let Remote;
    try {
        Remote = require("remote");
    } catch (err) {
        Remote = null;
    }

    if (Remote) {
        // atom-shell
        let app = Remote.require("app");
        let mainWin = Remote.require("browser-window").fromId(1);

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
        console.warn("Atom-shell support disabled.");
    }
})();
