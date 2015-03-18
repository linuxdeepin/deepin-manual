"use strict";

var URL = require("url-parse");

var splitPathFileNames = function(pf) {
    var i = pf.lastIndexOf("/");
    if (i >= 0) {
        return [pf.substr(0, i), pf.substr(i)];
    } else {
        throw new Error("Cannot split" + pf);
    }
};

var getDManFileInfo = function(url, lang) {
    url = url.trim();

    var parsed = new URL(url);
    if (parsed.protocol === "file:") {
        parsed.protocol = "";
        parsed.href = parsed.href.substr("file://".length);
    }
    var query = URL.qs.parse(parsed.query);

    var result = {
        lang: null,
        baseDir: null,
        dir: null,
        hash:  parsed.hash.substr(1) || null,
    };
    result.lang = query.lang || lang || null;


    switch (parsed.protocol) {
        case "dman:":
            result.baseDir = ["/usr/share/dman", parsed.hostname].join("/");
            if (!result.lang) {
                throw new Error("dman scheme must provide a language code");
            }
            result.dir = [result.baseDir, result.lang].join("/");
            break;
        case "":
            result.baseDir = splitPathFileNames(parsed.pathname)[0];
            result.dir = result.baseDir;
            break;
        case "http:":
        case "https:":
            var parts = parsed.pathname.split("/");
            result.lang = parts[parts.length - 2];
            result.baseDir = [
                parsed.protocol,
                "//",
                parsed.host,
                parts.slice(0, parts.length - 2).join("/"),
            ].join("");
            result.dir = [
                parsed.protocol,
                "//",
                parsed.host,
                parts.slice(0, parts.length - 2).join("/"),
                "/",
                result.lang,
            ].join("");

            break;
        default:
            throw new Error("Unknown protocol");
            break;
    }

    return result;
};

let getContentStylePath = function(indexPath) {
    let parsed = new URL(indexPath);
    if (parsed.protocol === "http:" || parsed.protocol === "https:") {
        return [parsed.protocol + "/", parsed.host + splitPathFileNames(parsed.pathname)[0], "style"].join("/");
    } else if (parsed.protocol === "file:" || parsed.protocol === "") {
        return [splitPathFileNames(parsed.pathname)[0], "style"].join("/");
    } else {
        throw new Error("Unknown protocol.");
    }
};

if (typeof exports !== "undefined") {
    exports.getDManFileInfo = getDManFileInfo;
    exports.getContentStylePath = getContentStylePath;
}
