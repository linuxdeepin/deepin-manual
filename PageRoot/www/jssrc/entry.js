"use strict";

var angular = require("angular");
var marked = require("marked");
var urlparse = require("url-parse");
var renderer = require("./renderer");
var utils = require("./utils");


if (!window.require) {
    window.require = require;
}
