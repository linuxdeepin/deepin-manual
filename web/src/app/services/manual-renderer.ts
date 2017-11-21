
import * as marked from 'marked';
import * as urlparse from 'url-parse';

const MAX_INDEX_HEADER_LEVEL = 4;
const MAX_NAV_HEADER_LEVEL = 3;
const EXTERNAL_READER_NODE_MODE_SPECIAL_ID = ' NODE ';

const normalizeAnchorName = function(raw) {
  // Convert any *space characters* per:
  // http://www.w3.org/TR/html5/dom.html#the-id-attribute
  // http://www.w3.org/TR/html5/infrastructure.html#space-character
  // replace spaces to '-'
  return raw
    .replace(/[\u0020|\u0009|\u000A|\u000C|\u000D]+/g, '-');
};

const addAnchor = function(target, anchorText, anchorId, icon) {
  // push toAdd to target
  const toAdd = Object.create(null);
  toAdd.text = anchorText;
  toAdd.id = anchorId;
  toAdd.icon = icon;
  toAdd.smallIcon = getSmallSvg(icon);
  toAdd.children = [];
  target.push(toAdd);
};

const findAnchor = function(target, anchorId) {
  for (let i = 0; i <= target.length; i++) {
    if (target[i].id === anchorId) {
      return target[i];
    }
  }
  throw new Error(`Cannot find anchor ${anchorId}.`);
};

const getSmallSvg = function(url) {
  if (!url) {
    return null;
  }
  const i = url.lastIndexOf('.');
  return url.substr(0, i) + '_small' + url.substr(i);
};

const parseNavigationItems = function(tokens) {
  const anchors = []; // This is the return value.
  const anchorIds = new Set(); // This is used to detect duplicate anchor ids.

  let currentHeaderId = null;
  let currentHeaderId1 = null;  // h1 header.
  let currentHeaderText = null;

  const appInfo = {
    name: null,
    icon: null,
  };
  const indices = [];
  const headers = [];
  const _inlineDeMarkdown = function(token) {
    // called by parseNavigationItems,
    // html, paragraph, and text needs inline parsing to strip markdown marks.
    const payload = [token];
    payload.links = tokens.links;
    const parser = new marked.Parser({
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
    const text = parser.parse(payload);
    return text;
  };

  const _addText = function(what) {
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

  for (const token of tokens) {
    switch (token.type) {
      case 'heading': {
        const extracted = extractHeaderIcon(token.text, token.depth);
        const anchorText = extracted.text;
        const icon = extracted.icon;

        // Lookfor and set appName
        if (token.depth === 1) {
          if (appInfo.name) {
            throw new Error(`Redefinition appInfo: ${appInfo.name} ${anchorText}`);
          }
          appInfo.name = anchorText;
          appInfo.icon = icon;
        }

        const anchorId = normalizeAnchorName(anchorText);

        if (token.depth <= MAX_INDEX_HEADER_LEVEL) {
          if (anchorIds.has(anchorId)) {
            throw new Error(`Duplicate anchor names found: ${anchorId}`);
          } else {
            anchorIds.add(anchorId);
            headers.push(anchorText);
          }
        } else {
          // continue;
        }
        currentHeaderId = anchorId;
        currentHeaderText = anchorText;

        if (token.depth === 2) {
          if (!appInfo.name) {
            throw new Error('H2 must be under H1.');
          }
          currentHeaderId1 = anchorId;
          addAnchor(anchors, anchorText, anchorId, icon);
        } else if (token.depth === 3) {
          const anchor_lv1 = findAnchor(anchors, currentHeaderId1);
          addAnchor(anchor_lv1.children, anchorText, anchorId, icon);
        }
        break;
      }
      case 'paragraph': {
        const firstThree = token.text.substr(0, 3);
        let shouldBreak = false;
        switch (firstThree) {
          case '!←←':
          case '←!→':
          case '→→!': {
            shouldBreak = true;
            break;
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
      case 'text':
      case 'html':
      {
        _addText(_inlineDeMarkdown(token));
        break;
      }
      case 'code': {
        _addText(_inlineDeMarkdown(token));
        break;
      }
      case 'list_start':
      case 'list_end':
      case 'list_item_start':
      case 'list_item_end':
      case 'loose_item_start':
      case 'loose_item_end':
      case 'blockquote_start':
      case 'blockquote_end':
      case 'space': {
        // pass
        break;
      }
      default: {
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
  };
};

const extractHeaderIcon = function(text, level) {
  let icon = null;
  if (level >= MAX_NAV_HEADER_LEVEL) {
    return {
      icon: icon,
      text: text,
    };
  }
  const re = /\|\S+\|$/;
  const matches = re.exec(text);
  if (matches && matches.length > 0) {
    const match = matches[0];
    icon = match.substring(1, match.length - 1);
    text = text.substring(0, text.length - match.length);
  }
  return {
    icon: icon,
    text: text,
  };
};

const extractImageLayout = function(text) {
  const re = /^\d+\|/;
  const matches = re.exec(text);
  let layout;
  if (matches && matches.length > 0) {
    const match = matches[0];
    text = text.substr(match.length);
    layout = parseInt(match);
  }
  return {
    layout: layout,
    text: text,
  };
};

const noop = () => '';

const getPlainRenderer = function() {
  const renderer = new marked.Renderer();
  renderer.heading = noop;
  renderer.image = function(href, title, text) {
    const tmp = extractImageLayout(text);
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
  // renderer.hr;
  // renderer.br;
  renderer.list = function(body, ordered) {
    return body;
  };
  renderer.listitem = function(text) {
    return text;
  };
  renderer.paragraph = function(text) {
    const firstThree = text.substr(0, 3);
    let shouldBreak = false;
    switch (firstThree) {
      case '!←←':
      case '←!→':
      case '→→!': {
        shouldBreak = true;
      }
      default: {}
    }
    if (shouldBreak) {
      return '';
    }
    return text;
  };

  return renderer;
};

const getHTMLRenderer = function() {
  const renderer = new marked.Renderer();

  renderer.heading = function(text, level, raw) {
    if (level === 1) {
      return '';
    }
    if (level > MAX_INDEX_HEADER_LEVEL) {
      // Do not give h5, h6 anchors.
      return `<h${level}>${text}</h${level}>\n`;
    }

    const extracted = extractHeaderIcon(text, level);

    return `<h${level} id="${normalizeAnchorName(extracted.text)}">${extracted.text}</h${level}>\n`;
  };

  renderer.image = function(href, title, text) {
    const tmp = extractImageLayout(text);
    const layout = tmp.layout;
    text = tmp.text;

    let out = `<img src="${href}" alt="${text}"`;
    if (title) {
      out += ` title="${title}"`;
    }

    if (typeof layout === 'number') {
      if (layout > 0) {
        out += ` class="block${layout}"`;
      }
    } else {
      out += ' class="inline"';
      const heightExtractor = /\S+\-([0-9]+).svg/;
      const result = heightExtractor.exec(href);
      if (result) {
        const height = parseInt(result[1]);
        if (height) {
          out += ` style="height:${height}px;"`;
        }
      }
    }
    out += this.options.xhtml ? '/>' : '>';
    return out;
  };

  renderer.link = function(href, title, text) {
    let klass = '';
    title = title || '';
    let inLinkExtra = '';
    let onclick = '';

    if (href.indexOf('#') === 0) {
      href = `javascript: window.parent.jumpTo('${normalizeAnchorName(href.substring(1))}');`;
    } else if (href.indexOf('dman:///') === 0) {
      onclick = `window.parent.externalRead('${href}', this)`;
      href = `javascript: void(0)`;
      klass = 'external';
      inLinkExtra = `<span class="icon"></span>`;
    }
    const out = `<a href="${href}" title="${title}" class="${klass}" onclick="${onclick}"><span class="text">${text}</span>${inLinkExtra}</a>`;
    return out;
  };

  renderer.paragraph = function(text) {
    const firstThree = text.substr(0, 3);
    switch (firstThree) {
      case '!←←': {
        return `<table class="columns"><tbody><tr><td>`;
      }
      case '←!→': {
        return `</td><td>`;
      }
      case '→→!': {
        return `</td></tr></tbody></table>\n`;
      }
      default: {
        return `<p>${text}</p>\n`;
      }
    }
  };
  return renderer;
};

const processMarkdown = function(src) {
  // Lex
  const lexer = new marked.Lexer();
  let tokens;
  try {
    tokens = lexer.lex(src);
  } catch (err) {
    throw new Error(`Lexer Error ${err}`);
  }

  // Extract Headers
  const parsed = parseNavigationItems(tokens);

  // Pass tokens to HTML renderer
  const html = marked(src, {
    renderer: getHTMLRenderer(),
  });

  return {
    html: html,
    parsed: parsed,
  };
};

// External Reader
const extractTokenRange = function(tokens, fromHeaderId, toHeaderId) {
  // returns a list of tokens
  const result = [];
  let rangeStartLevel = 0;
  let inRange = false;
  tokenLoop: for (const token of tokens) {
    if (token.type === 'heading') {
      const headerText = extractHeaderIcon(token.text, token.depth).text;
      const headerId = normalizeAnchorName(headerText);
      if ((toHeaderId === EXTERNAL_READER_NODE_MODE_SPECIAL_ID) && (token.depth <= rangeStartLevel)) {
        inRange = false;
        break tokenLoop;
      }
      if (headerId === fromHeaderId) {
        inRange = true;
        rangeStartLevel = token.depth;
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

const getHTMLRendererForExternalBase = function() {
  const renderer = getHTMLRenderer();
  renderer.image = function(href, title, text) {
    const tmp = extractImageLayout(text);
    const layout = tmp.layout;
    text = tmp.text;

    if (!layout) {
      if (layout !== 0) {
        return `<img src="${href}" alt="${text}" title="${title}" class="inline" />`;
      }
      throw new Error('No output: unscaled images');
    }
    throw new Error('No output: Non-inline images');
  };
  renderer.link = (href, title, text) => text;
  return renderer;
};

const getHTMLRendererForExternalRange = function() {
  const renderer = getHTMLRendererForExternalBase();
  return renderer;
};

const getHTMLRendererForExternalNode = function() {
  const renderer = getHTMLRendererForExternalRange();
  renderer.heading = (text, level, raw) => {
    const extracted = extractHeaderIcon(text, level);
    return `<header id="${normalizeAnchorName(extracted.text)}">${extracted.text}</header>\n`;
  };
  return renderer;
};

const extractExternalHtml = function(src, fromHeaderId, toHeaderId) {
  const lexer = new marked.Lexer();
  let tokens;
  try {
    tokens = lexer.lex(src);
  } catch (err) {
    throw new Error(`Lexer Error ${err}`);
  }
  tokens = extractTokenRange(tokens, fromHeaderId, toHeaderId);

  // render
  let renderer;
  if (toHeaderId === EXTERNAL_READER_NODE_MODE_SPECIAL_ID) {
    renderer = getHTMLRendererForExternalNode();
  } else {
    renderer = getHTMLRendererForExternalRange();
  }
  const parser = new marked.Parser({
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
  const oldTok = parser.tok;
  parser.tok = function() {
    try {
      return oldTok.call(this);
    } catch (err) {
      return '';
    }
  };
  return parser.parse(tokens);
};

const parseExternalLink = function(link, baseDManDir) {
  // parse a link in the format of "dman://<AppName>#fromAnchorText|toAnchorText"
  const parsed = urlparse(link);

  // process the url of the external manual
  const refName = parsed.pathname.substr(1);
  const parts = baseDManDir.split('/');
  parts[parts.length - 2] = refName;
  const markdownDir = parts.join('/');

  let fromHeaderId = null;
  let toHeaderId = null;

  // process the anchors of the external manual
  const hash = parsed.hash.substr(1);  // get rid of #
  if (hash[0] === '[' && hash[hash.length - 1] === '}') {
    const rangeParts = hash.split('|');
    if (rangeParts.length !== 2) {
      throw new Error(`Multiple delimiters(|) found in the external link: ${link}`);
    }
    fromHeaderId = normalizeAnchorName(rangeParts[0].substr(1));
    toHeaderId = normalizeAnchorName(rangeParts[1].substr(0, rangeParts[1].length - 1)) || null;
  } else {
    // Node mode
    fromHeaderId = normalizeAnchorName(hash);
    toHeaderId = EXTERNAL_READER_NODE_MODE_SPECIAL_ID;
  }

  return {
    markdownDir: markdownDir,
    fromHeaderId: fromHeaderId,
    toHeaderId: toHeaderId,
  };
};

export {
  normalizeAnchorName,
  getHTMLRenderer,
  getPlainRenderer,
  processMarkdown,
  getSmallSvg,
  extractTokenRange,
  getHTMLRendererForExternalBase,
  getHTMLRendererForExternalRange,
  getHTMLRendererForExternalNode,
  parseExternalLink,
  extractExternalHtml
};
