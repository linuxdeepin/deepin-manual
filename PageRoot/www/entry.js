"use strict";

var angular = require("angular");
var marked = require("marked");
var urlparse = require("url-parse");

if (!window.require) {
    window.require = require;
}
