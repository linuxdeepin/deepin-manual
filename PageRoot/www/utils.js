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

/**
 *
 * @param url: a url that points to a dman file or a dman directory. It can be one of the following:
 *             dman, http, https, file scheme.
 * @param lang optional. can be used to determine which language code to use.
 * @returns {{absPath: string, version: string, languageDetermined: boolean}}
 */
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
            console.log("=================", parts);
            console.log(parts.slice(0, parts.length - 2));
            result.lang = parts[parts.length - 2];
            result.dir = [
                parsed.protocol,
                "//",
                parsed.host,
                parts.slice(0, parts.length - 2).join("/"),
                result.lang,
            ].join("");

            break;
        default:
            throw new Error("Unknown protocol");
            break;
    }

    return result;
};

if (typeof exports !== "undefined") {
    exports.getDManFileInfo = getDManFileInfo;
}