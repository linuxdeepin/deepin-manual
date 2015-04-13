"use strict";

let URL = require("url-parse");

let splitPathFileNames = function(pf) {
    let i = pf.lastIndexOf("/");
    if (i >= 0) {
        return [pf.substr(0, i), pf.substr(i)];
    } else {
        throw new Error("Cannot split" + pf);
    }
};

let getDManFileInfo = function(url, lang) {
    url = url.trim();

    let parsed = new URL(url);
    if (parsed.protocol === "file:") {
        parsed.protocol = "";
        parsed.href = parsed.href.substr("file://".length);
    }
    let query = URL.qs.parse(parsed.query);

    let result = {
        lang: null,
        baseDir: null,
        dir: null,
        hash: parsed.hash.substr(1) || null,
    };
    result.lang = query.lang || lang || null;


    switch (parsed.protocol) {
        case "dman:":
            result.baseDir = ["file:/", "/usr/share/dman", parsed.hostname].join("/");
            if (!result.lang) {
                throw new Error("dman scheme must provide a language code");
            }
            result.dir = [result.baseDir, result.lang].join("/");
            break;
        case "":
            result.baseDir = "file://" + splitPathFileNames(parsed.pathname)[0];
            result.dir = result.baseDir;
            break;
        case "http:":
        case "https:": {
            let parts = parsed.pathname.split("/");
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
        }
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
        return ["file:/", splitPathFileNames(parsed.pathname)[0], "style"].join("/");
    } else {
        throw new Error("Unknown protocol.");
    }
};

let searchHighlight = function(text, keywords) {
    if (keywords.length === 0) {
        return "";
    }
    let pattern = [];
    for (let keyword of keywords) {
        pattern.push(keyword.replace(/([[^$.|?*+(){}])/g, '\\$1'));
    }
    pattern = "(?:" + pattern.join(")|(?:") + ")";
    pattern = new RegExp(pattern, "gi");

    let foundFlag = false;
    let result = text.replace(pattern, function(match){
        foundFlag = true;
        return `<span class="highlight">${match}</span>`;
    });
    return foundFlag ? result : "";
};

/**
 * (Used in angular filter: filterHighlight)
 * @param filtered an array of AnchorItem's.
 * @param anchorId string
 * @returns {*}
 */
let getAnchorItem = function(filtered, anchorId) {
    for (let item of filtered) {
        if (item.anchorId === anchorId) {
            return item;
        }
    }
    return null;
};

if (typeof exports !== "undefined") {
    exports.searchHighlight = searchHighlight;
    exports.getDManFileInfo = getDManFileInfo;
    exports.getContentStylePath = getContentStylePath;
    exports.getAnchorItem = getAnchorItem;
}
