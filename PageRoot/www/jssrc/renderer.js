"use strict";

// Make a custom version of the marked renderer.

let getDManFileInfo = require("./utils").getDManFileInfo;

let marked = require("marked");
let MAX_INDEX_HEADER_LEVEL = 2;
let MAX_NAV_HEADER_LEVEL = 3;

let normalizeAnchorName = function(raw) {
    return raw
        // Convert any *space characters* per:
        // http://www.w3.org/TR/html5/dom.html#the-id-attribute            and
        // http://www.w3.org/TR/html5/infrastructure.html#space-character
        .replace(/[\u0020|\u0009|\u000A|\u000C|\u000D]+/g, '-');  // replace spaces to '-'
};

// Keep track of navigation items;
// Keep track of keyword -> navigation item lookup table.


let addAnchor = function(target, anchorText, anchorId, icon) {
    // push toAdd to target
    let toAdd = Object.create(null);
    toAdd.text = anchorText;
    toAdd.id = anchorId;
    toAdd.icon = icon;
    toAdd.children = [];
    target.push(toAdd);
};

let findAnchor = function(target, anchorId) {
    for (let i = 0; i <= target.length; i++) {
        if (target[i].id === anchorId) {
            return target[i];
        }
    }
    throw new Error(`Cannot find anchor ${anchorId}.`);
};

let parseNavigationItems = function(tokens) {
    let anchors = []; // This is the return value.
    let anchorIds = new Set(); // This is used to detect duplicate anchor ids.

    let currentHeaderId = null;
    let currentHeaderId1 = null;  // h1 header.
    let currentHeaderText = null;

    let appInfo = {
        name: null,
        icon: null,
    };
    let indices = [];
    let headers = [];
    let _addText = function(what) {
        if ((indices.length === 0) ||
            (indices[indices.length - 1].header !== currentHeaderId)) {
            indices.push({
                headerId: currentHeaderId,
                headerText: currentHeaderText,
                texts: [],
            });
        }
        indices[indices.length - 1].texts.push(what);
    };

    for (let token of tokens) {
        switch (token.type) {
            case "heading": {
                let extracted = extractHeaderIcon(token.text, token.depth);
                let anchorText = extracted.text;
                let icon = extracted.icon;

                // Lookfor and set appName
                if (token.depth === 1) {
                    if (appInfo.name) {
                        throw new Error(`Redefinition appInfo: ${appInfo.name} ${anchorText}`);
                    }
                    appInfo.name = anchorText;
                    appInfo.icon = icon;
                }

                let anchorId = normalizeAnchorName(anchorText);

                if (token.depth <= MAX_INDEX_HEADER_LEVEL) {
                    if (anchorIds.has(anchorId)) {
                        throw new Error("Duplicate anchor names found:", anchorId);
                    } else {
                        anchorIds.add(anchorId);
                        headers.push(anchorText);
                    }
                } else {
                    //continue;
                }
                currentHeaderId = anchorId;
                currentHeaderText = anchorText;

                if (token.depth === 2) {
                    if (!appInfo.name) {
                        throw new Error("H2 must be under H1.");
                    }
                    currentHeaderId1 = anchorId;
                    addAnchor(anchors, anchorText, anchorId, icon);
                } else if (token.depth === 3) {
                    let anchor_lv1 = findAnchor(anchors, currentHeaderId1);
                    addAnchor(anchor_lv1.children, anchorText, anchorId, icon);
                }
                break;
            }
            case "paragraph": {
                _addText(token.text);
                break;
            }
            case "text":
            {
                let payload = [token];
                payload.links = tokens.links;
                let parser = new marked.Parser({
                    gfm: true,
                    tables: true,
                    breaks: false,
                    pedantic: false,
                    sanitize: false,
                    smartLists: false,
                    silent: false,
                    highlight: null,
                    langPrefix: 'lang-',
                    smartypants: false,
                    headerPrefix: '',
                    renderer: getPlainRenderer(),
                    xhtml: false,
                });
                let text = parser.parse(payload);
                _addText(text);
                break;
            }
            case "html": {
                console.warn("TODO: html detagging");
                break;
            }
            case "code": {
                console.warn(`TODO: code ${token.text}`);
                break;
            }
            case "list_start":
            case "list_end":
            case "list_item_start":
            case "list_item_end":
            case "loose_item_start":
            case "loose_item_end":
            case "blockquote_start":
            case "blockquote_end":
            case "space": {
                // pass
                break;
            }
            default:
            {
                console.warn(`Unhandled token ${token.type}`);
                break;
            }
        }
    }

    return {
        appInfo: appInfo,  // set logo, icon, and window's title
        anchors: anchors,  // for navigation purposes
        indices: indices,  // for indexing and searching reasons
        headers: headers,  // for search auto-completion
    }
};

let extractHeaderIcon = function(text, level) {
    let icon = null;
    if (level >= MAX_NAV_HEADER_LEVEL) {
        return {
            icon: icon,
            text: text,
        }
    }
    let re = /\|\S+\|$/;
    let matches = re.exec(text);
    if (matches && matches.length > 0) {
        let match = matches[0];
        icon = match.substring(1, match.length - 1);
        text = text.substring(0, text.length - match.length);
    }
    return {
        icon: icon,
        text: text,
    }
};

let extractImageLayout = function(text) {
    let re = /^\d+\|/;
    let matches = re.exec(text);
    let layout;
    if (matches && matches.length > 0) {
        let match = matches[0];
        text = text.substr(match.length);
        layout = parseInt(match);
    }
    return {
        layout: layout,
        text: text,
    }
};

let getPlainRenderer = function() {
    let renderer = new marked.Renderer();
    let noop = function() { return ""; };

    renderer.heading = noop;
    renderer.image = function(href, title, text) {
        let tmp = extractImageLayout(text);
        return tmp.text;
    };
    renderer.table = function(header, body) {
        return `${header} ${body}`;
    };
    renderer.tablerow = function(content) {
        return content;
    };
    renderer.tablecell = function(content, flags) {
        return content;
    };
    renderer.strong = function(text) {
        return text;
    };
    renderer.em = function(text) {
        return text;
    };
    renderer.codespan = function(text) {
        return text;
    };
    renderer.del = function(text) {
        return text;
    };
    renderer.link = function(href, title, text) {
        return text;
    };
    renderer.blockquote = function(quote) {
        return quote;
    };
    renderer.code = function(code, lang, escaped) {
        return code;
    };
    renderer.html = function(html) {
        return html;
    };
    //renderer.hr;
    //renderer.br;
    renderer.list = function(body, ordered) {
        return body;
    };
    renderer.listitem = function(text) {
        return text;
    };
    renderer.paragraph = function(text) {
        return text;
    };

    return renderer;
};

let getHTMLRenderer = function() {
    let renderer = new marked.Renderer();

    renderer.heading = function(text, level, raw) {
        if (level > MAX_NAV_HEADER_LEVEL) {
            // Do not give h4, h5, h6 anchors.
            return '<h' + level + '>'
                + text
                + '</h' + level + '>\n';
        }

        let result = "";

        let extracted = extractHeaderIcon(text, level);

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
        let tmp = extractImageLayout(text);
        let layout = tmp.layout;
        text = tmp.text;

        let out = '<img src="' + href + '" alt="' + text + '"';
        if (title) {
            out += ' title="' + title + '"';
        }
        if (layout) {
            out += ' class="block' + layout + '"';
        } else {
            if (layout === 0) {

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
        let out = '<a href="' + href + '"';
        if (title) {
            out += ' title="' + title + '"';
        }
        out += '>' + text + '</a>';
        return out;
    };
    return renderer;
};

let processMarkdown = function(src) {
    // Lex
    let lexer = new marked.Lexer();
    let tokens;
    try {
        tokens = lexer.lex(src);
    } catch (err) {
        throw new Error(`Lexer Error ${err}`);
    }

    // Extract Headers
    let parsed = parseNavigationItems(tokens);

    // Pass tokens to HTML renderer
    let html = marked(src, {
        renderer: getHTMLRenderer(),
    });

    return {
        html: html,
        parsed: parsed,
    }

};

if (typeof exports !== "undefined") {
    exports.getHTMLRenderer = getHTMLRenderer;
    exports.getPlainRenderer = getPlainRenderer;
    exports.processMarkdown = processMarkdown;
}
