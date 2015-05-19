"use strict";

let marked = require("marked");
let urlparse = require("url-parse");
let MAX_INDEX_HEADER_LEVEL = 4;
let MAX_NAV_HEADER_LEVEL = 3;

let normalizeAnchorName = function(raw) {
    return raw
        // Convert any *space characters* per:
        // http://www.w3.org/TR/html5/dom.html#the-id-attribute            and
        // http://www.w3.org/TR/html5/infrastructure.html#space-character
        .replace(/[\u0020|\u0009|\u000A|\u000C|\u000D]+/g, '-');  // replace spaces to '-'
};

let addAnchor = function(target, anchorText, anchorId, icon) {
    // push toAdd to target
    let toAdd = Object.create(null);
    toAdd.text = anchorText;
    toAdd.id = anchorId;
    toAdd.icon = icon;
    toAdd.smallIcon = getSmallSvg(icon);
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

let getSmallSvg = function(url) {
    if (!url) {
        return null;
    }
    let i = url.lastIndexOf(".");
    return url.substr(0, i) + "_small" + url.substr(i);
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
    let _inlineDeMarkdown = function(token) {
        // called by parseNavigationItems,
        // html, paragraph, and text needs inline parsing to strip markdown marks.
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
        return text;
    };

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
                        throw new Error(`Duplicate anchor names found: ${anchorId}`);
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
                let firstThree = token.text.substr(0, 3);
                let shouldBreak = false;
                switch (firstThree) {
                    case "!←←":
                    case "←!→":
                    case "→→!": {
                        shouldBreak = true;
                    }
                    default: {}
                }
                if (shouldBreak) {
                    // columns control characters
                    break;
                }
                _addText(_inlineDeMarkdown(token));
                break;
            }
            case "text":
            case "html":
            {
                _addText(_inlineDeMarkdown(token));
                break;
            }
            case "code": {
                _addText(_inlineDeMarkdown(token));
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
        return html.replace(/<[^>]*>/g, ' ').replace(/\s{2,}/g, ' ').trim();
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
        let firstThree = text.substr(0, 3);
        let shouldBreak = false;
        switch (firstThree) {
            case "!←←":
            case "←!→":
            case "→→!": {
                shouldBreak = true;
            }
            default: {}
        }
        if (shouldBreak) {
            return "";
        }
        return text;
    };

    return renderer;
};

let getHTMLRenderer = function() {
    let renderer = new marked.Renderer();

    renderer.heading = function(text, level, raw) {
        if (level === 1) {
            return '';
        }
        if (level > MAX_NAV_HEADER_LEVEL) {
            // Do not give h4, h5, h6 anchors.
            return `<h${level}>${text}</h${level}>\n`;
        }

        let extracted = extractHeaderIcon(text, level);

        return `<h${level} id="${normalizeAnchorName(extracted.text)}">${extracted.text}</h${level}>\n`;
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
        let klass = "";
        title = title || "";
        let inLinkExtra = "";
        let onclick = "";

        if (href.indexOf("#") === 0) {
            href = `javascript: window.parent.jumpTo('${href.substring(1)}');`;
        } else if (href.indexOf("dman:///") === 0) {
            onclick = `window.parent.externalRead('${href}', this)`;
            href = `javascript: void(0)`;
            klass = "external";
            inLinkExtra = `<span class="icon"></span>`;
        }
        let out = `<a href="${href}" title="${title}" class="${klass}" onclick="${onclick}">${text}${inLinkExtra}</a>`;
        return out;
    };

    renderer.paragraph = function(text) {
        let firstThree = text.substr(0, 3);
        switch (firstThree) {
            case "!←←": {
                return `<table class="columns"><tbody><tr><td>`;
            }
            case "←!→": {
                return `</td><td>`;
            }
            case "→→!": {
                return `</td></tr></tbody></table>\n`;
            }
            default: {
                return `<p>${text}</p>\n`;
            }
        }
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

// External Reader
let extractTokenRange = function(tokens, fromHeaderId, toHeaderId) {
    // returns a list of tokens
    let result = [];
    let inRange = false;
    tokenLoop: for (let token of tokens) {
        if (token.type === "heading") {
            let headerText = extractHeaderIcon(token.text, token.depth).text;
            let headerId = normalizeAnchorName(headerText);
            if (headerId === fromHeaderId) {
                inRange = true;
            }
            if (headerId === toHeaderId) {
                inRange = false;
                break tokenLoop;
            }
        }
        if (inRange) {
            result.push(token);
        }
    }
    // preserve the original links property
    result.links = tokens.links;
    return result;
};

let extractExternalHtml = function(src, fromHeaderId, toHeaderId) {
    let lexer = new marked.Lexer();
    let tokens;
    try {
        tokens = lexer.lex(src);
    } catch (err) {
        throw new Error(`Lexer Error ${err}`);
    }
    tokens = extractTokenRange(tokens, fromHeaderId, toHeaderId);

    // render
    let renderer = getHTMLRenderer();
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
        renderer: renderer,
        xhtml: false,
    });
    return parser.parse(tokens);
};

let parseExternalLink = function(link, baseDManDir) {
    // parse a link in the format of "dman://<AppName>#fromAnchorText|toAnchorText"
    let parsed = urlparse(link);

    // process the url of the external manual
    let refName = parsed.pathname.substr(1);
    let parts = baseDManDir.split("/");
    parts[parts.length - 2] = refName;
    let markdownDir = parts.join("/");

    // process the anchors of the external manual
    let hash = parsed.hash.substr(1);  // get rid of #
    if (hash[0] !== "[" || hash[hash.length - 1] !== "}") {
        throw new SyntaxError(`Only Inclusive...Exclusive range is supported.`);
    }

    let rangeParts = hash.split("|");
    if (rangeParts.length !== 2) {
        throw new Error(`Multiple delimiters(|) found in the external link: ${link}`);
    }
    let fromHeaderId = normalizeAnchorName(rangeParts[0].substr(1));
    let toHeaderId = normalizeAnchorName(rangeParts[1].substr(0, rangeParts[1].length - 1)) || null;

    return {
        markdownDir: markdownDir,
        fromHeaderId: fromHeaderId,
        toHeaderId: toHeaderId,
    };
};

if (typeof exports !== "undefined") {
    exports.normalizeAnchorName = normalizeAnchorName;
    exports.getHTMLRenderer = getHTMLRenderer;
    exports.getPlainRenderer = getPlainRenderer;
    exports.processMarkdown = processMarkdown;
    exports.getSmallSvg = getSmallSvg;
    exports.extractTokenRange = extractTokenRange;
    exports.parseExternalLink = parseExternalLink;
    exports.extractExternalHtml = extractExternalHtml;
}
