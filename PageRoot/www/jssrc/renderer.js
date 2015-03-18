"use strict";

// Make a custom version of the marked renderer.
var marked = require("marked");
var MAX_INDEX_HEADER_LEVEL = 2;
var MAX_NAV_HEADER_LEVEL = 3;

var normalizeAnchorName = function(raw) {
    return raw.toLowerCase()
              // Convert any *space characters* per:
              // http://www.w3.org/TR/html5/dom.html#the-id-attribute            and
              // http://www.w3.org/TR/html5/infrastructure.html#space-character
              .replace(/[\u0020|\u0009|\u000A|\u000C|\u000D]+/g, '-');  // replace spaces to '-'
};

// Keep track of navigation items;
// Keep track of keyword -> navigation item lookup table.


var addItem = function(target, name, anchor, icon) {
    // push toAdd to target
    var toAdd = Object.create(null);
    toAdd.name = name;
    toAdd.anchor = anchor;
    toAdd.icon = icon;
    toAdd.children = [];
    target.push(toAdd);
};

var findItem = function(target, anchorName) {
    var result = null;
    for (var i = 0; i <= target.length; i++) {
        if (target[i].anchor === anchorName) {
            return target[i];
        }
    }
    throw new Error("Cannot find Item", anchorName);
};

var parseNavigationItems = function(tokens) {
    var items = []; // This is the return value.
    var anchorSet = new Set(); // This is used to detect duplicate anchor names.

    var currentHeader = null;
    var currentHeader1 = null;  // h1 header.

    var appInfo = {
        name: null,
        icon: null,
    };
    var indices = [];
    var headers = [];

    for (var token of tokens) {
        if (token.type === "heading") {
            var extracted = extractHeaderIcon(token.text, token.depth);
            var text = extracted.text;
            var icon = extracted.icon;

            // Lookfor and set appName
            if (token.depth === 1) {
                if (appInfo.name) {
                    throw new Error("Redefinition appInfo");
                }
                appInfo.name = text;
                appInfo.icon = icon;
            }

            var anchorName = normalizeAnchorName(text);

            if (token.depth <= MAX_INDEX_HEADER_LEVEL) {
                if (anchorSet.has(anchorName)) {
                    throw new Error("Duplicate anchor names found:", anchorName);
                } else {
                    anchorSet.add(anchorName);
                    headers.push(text);
                }
            } else {
//                continue;
            }
            currentHeader = anchorName;
            if (token.depth === 2) {
                if (!appInfo.name) {
                    throw new Error("H2 must be under H1.");
                }
                currentHeader1 = anchorName;

                addItem(items, text, anchorName, icon);
            } else if (token.depth === 3) {
                var header1 = findItem(items, currentHeader1);
                addItem(header1.children, text, anchorName, icon);
            }

        } else {
            if (token.type === "paragraph" || token.type === "text") {
                if ((indices.length === 0) ||
                    (indices[indices.length - 1].header !== currentHeader)) {
                    indices.push({
                        header: currentHeader,
                        texts: [],
                    })
                }
                indices[indices.length - 1].texts.push(token.text);
            }
        }

    }

    return {
        appInfo: appInfo,
        items: items,
        indices: indices,
        headers: headers,
    }
};

var extractHeaderIcon = function(text, level) {
    var icon = null;
    if (level >= MAX_NAV_HEADER_LEVEL) {
        return {
            icon: icon,
            text: text,
        }
    }
    var re = new RegExp(/\|\S+\|$/);
    var matches = re.exec(text);
    if (matches && matches.length > 0) {
        var match = matches[0];
        icon = match.substring(1, match.length - 1);
        text = text.substring(0, text.length - match.length);
    }
    return {
        icon: icon,
        text: text,
    }
};

var getRenderer = function() {
    // export!
    var renderer = new marked.Renderer();

    renderer.heading = function(text, level, raw) {
        if (level > MAX_NAV_HEADER_LEVEL) {
            // Do not give h4, h5, h6 anchors.
            return '<h' + level + '>'
                + text
                + '</h' + level + '>\n';
        }

        var result = "";

        var extracted = extractHeaderIcon(text, level);

        result += '<h' + level + ' id="'
                + normalizeAnchorName(extracted.text)
                + '">';
        if (extracted.icon) {
            result += '<img class="HeaderIcon" src="' + extracted.icon + '" />';
        }
        result += extracted.text
                + '</h' + level + '>\n';
        return result;
    };

    renderer.image = function(href, title, text) {
        var re = /^\d+\|/;
        var matches = re.exec(text);
        if (matches && matches.length > 0) {
            var match = matches[0];
            text = text.substr(match.length);
            var imgNum = parseInt(match);
        }
        var out = '<img src="' + href + '" alt="' + text + '"';
        if (title) {
            out += ' title="' + title + '"';
        }
        if (imgNum) {
            out += ' class="block' + imgNum + '"';
        } else {
            if (imgNum === 0) {

            } else {
                out += ' class="inline"';
            }
        }
        out += this.options.xhtml ? '/>' : '>';
        return out;
    };

    renderer.link = function(href, title, text) {
        if (href.indexOf("#") === 0) {
            href = "javascript: window.parent.jumpTo('" + href.substring(1) + "');";
        }
        var out = '<a href="' + href + '"';
        if (title) {
            out += ' title="' + title + '"';
        }
        out += '>' + text + '</a>';
        return out;
    };
    return renderer;
};

var lookupKeyword = function(indices, keyword) {
    var result = [];
    for (var header in indices) {
        var sentences = indices[header];
        for (var sentence of sentences) {
            if (sentence.indexOf(keyword) >= 0) {
                result.push(header, sentence);
            }
        }
    }
    return result;
};

var loadMarkdown = function(url, callback) {
    if (typeof process !== "undefined") {
        // atom-shell
        var fs = require("fs");
        fs.readFile(url, function(error, data) {
            if (error) {
                callback(error, null);
            } else {
                callback(null, data.toString());
            }
        });
    } else if (typeof DAE !== "undefined") {
        // DAE
        let f = new DAE.File("/home/xinkai/projects/deepin-user-manual/PageRoot/www/manual/manual_zhCN.md");
        // let f = new DAE.File(url);
        callback(null, f.readText());
    } else if (typeof window !== "undefined") {
        // browser
        var xmlHttp = new XMLHttpRequest();
        xmlHttp.open("GET", url, true);
        xmlHttp.send();
        xmlHttp.onreadystatechange = function(target, type, bubbles, cancelable) {
            if (xmlHttp.readyState == 4 && xmlHttp.status == 200) {
                callback(null, xmlHttp.responseText);
            }
        };
        xmlHttp.onerror = function(event) {
            callback(new Error(event));
        };
    } else {
        callback(new Error("Cannot loadMarkdown because unable to detect runtime environment."),
                 null);
    }
};

var parseMarkdown = function(md) {

    var renderer = getRenderer();
    var html = marked(md, {
        renderer: renderer,
    });
    var lexer = new marked.Lexer({});
    var tokens = lexer.lex(md);

    var parsed = parseNavigationItems(tokens);
    return {
        parsed: parsed,
        html: html,
    }
};

if (typeof exports !== "undefined") {
    exports.getRenderer = getRenderer;
    exports.parseMarkdown = parseMarkdown;
}
