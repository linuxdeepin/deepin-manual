"use strict";

let {
    extractExternalHtml,
    normalizeAnchorName,
    parseExternalLink,
} = require("../renderer");

let {
    getContentStylePath,
    getScriptPath,
} = require("../utils");

angular.module("DManual").factory("ExternalReaderService", function(AdapterService, $log, GInput, $q, $window) {
    let _loadExternalDManual = function(refManualUrl, fromHeaderText, toHeaderText) {
        return $q(function(resolve, reject) {
            let fromHeaderId = fromHeaderText ? normalizeAnchorName(fromHeaderText) : null;
            let toHeaderId = toHeaderText ? normalizeAnchorName(toHeaderText) : null;

            GInput.load(`${refManualUrl}/index.md`).then(function(mdText) {
                let stylePath = getContentStylePath($window.location.href);
                let html = extractExternalHtml(mdText, fromHeaderId, toHeaderId);
                let fullHtml = `<base href='${refManualUrl}/'>
                <link rel='stylesheet' href='${stylePath}/reset.css' />
                <link rel='stylesheet' href='${stylePath}/content.css' />
                <link rel='stylesheet' href='${stylePath}/reader.css' />
                ${html}`;
                resolve(fullHtml);
            }, function(error) {
                reject(`Cannot load external markdown: ${error}`);
            });
        });
    };

    let _parseExternalLink = function(link) {
        // It does two things:
        // 1. get the url to the reference manual
        // 2. converts the anchor texts to anchor ids
        let baseMarkdownDir = AdapterService.markdownDir();
        return parseExternalLink(link, baseMarkdownDir);
    };
    return {
        loadExternalDManual: _loadExternalDManual,
        parseExternalLink: _parseExternalLink,
    };
});