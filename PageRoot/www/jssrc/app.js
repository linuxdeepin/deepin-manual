"use strict";

let app = angular.module("DManual", []);
app.filter("keyword", function() {
    let _PLACEHOLDER_RESULT = [];
    let lastTerm = null;
    let cached = null;
    return function(indices, keyword) {
        let result = [];
        if (!keyword) {
            return _PLACEHOLDER_RESULT;
        }
        if ((keyword === lastTerm) && cached) {
            return cached;
        }
        let lowerKeyword = keyword.toLowerCase();
        nextHeader: for (let index of indices) {
            let texts = index.texts;
            for (let text of texts) {
                let lowerText = text.toLowerCase();
                if (lowerText.indexOf(lowerKeyword) >= 0) {
                    result.push({
                        anchorId: index.headerId,
                        anchorText: index.headerText,
                        text: text,
                    });
                    continue nextHeader;
                }
            }
        }
        lastTerm = keyword;
        cached = result;
        return cached;
    }
});
