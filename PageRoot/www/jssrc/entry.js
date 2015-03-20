"use strict";

var angular = require("angular");
var marked = require("marked");
var urlparse = require("url-parse");
var renderer = require("./renderer");


if (!window.require) {
    window.require = require;
}
