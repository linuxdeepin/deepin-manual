"use strict";

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
        ipc.on("Markdown", function(path) {
            console.log("Markdown set to " + path);
        });

        // Atom-shell automatically injects the `close` method.
        //window.close = function() {
        //
        //};
    } else {
        console.warn("Atom-shell support disabled.");
    }
})();