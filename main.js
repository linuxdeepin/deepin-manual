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

app.commandLine.appendSwitch('remote-debugging-port', '18080');
app.commandLine.appendSwitch('host-rules', 'MAP * 127.0.0.1');

// This method will be called when atom-shell has done everything
// initialization and ready for creating browser windows.
app.on('ready', function () {
    // Create the browser window.
    mainWindow = new BrowserWindow({
        width: 946,
        "min-width": 946,
        height: 600,
        "min-height": 600,
        frame: false,
    });
    ipc.on("AdapterReady", function() {
        let argv = process.argv;
        let mdUrl = argv[argv.length - 1];
        if (mdUrl.indexOf("file://") !== 0) {
            mdUrl = "file://" + mdUrl;
        }
        mainWindow.send("setMarkdown", mdUrl);
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
