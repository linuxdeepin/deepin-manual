#!/usr/bin/atom-shell
"use strict";

let app = require('app');  // Module to control application life.
let BrowserWindow = require('browser-window');  // Module to create native browser window.
let ipc = require("ipc");

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the javascript object is GCed.
let mainWindow = null;

// Quit when all windows are closed.
app.on('window-all-closed', function () {
    if (process.platform !== 'darwin') {
        app.quit();
    }
});

// This method will be called when atom-shell has done everything
// initialization and ready for creating browser windows.
app.on('ready', function () {
    // Create the browser window.
    mainWindow = new BrowserWindow({
        width: 970,
        "min-width": 970,
        height: 600,
        "min-height": 600,
        frame: false,
    });
    ipc.on("AdapterReady", function() {
        let argv = process.argv;
        let mdUrl = argv[argv.length - 1];
        if (mdUrl.indexOf("http://") === 0 ||
            mdUrl.indexOf("https://") === 0||
            mdUrl.indexOf("file://") === 0 ||
            mdUrl.indexOf("dman://") === 0) {
            // pass
        } else {
            mdUrl = "file://" + require("path").resolve(mdUrl);
        }
        mainWindow.send("setMarkdown", {
            dmanDir: mdUrl,
            uiLangs: ["zh_CN"],
        });
        if (process.env["DEBUG"]) {
            mainWindow.openDevTools();
            mainWindow.send("Debug", true);
        }
    });



    console.log("BrowserWindow created with id", mainWindow.id);
    // and load the index.html of the app.
    mainWindow.loadUrl('file://' + __dirname + "/PageRoot/www/index.html");

    // Emitted when the window is closed.
    mainWindow.on('closed', function () {
        // Dereference the window object, usually you would store windows
        // in an array if your app supports multi windows, this is the time
        // when you should delete the corresponding element.
        mainWindow = null;
    });
});
