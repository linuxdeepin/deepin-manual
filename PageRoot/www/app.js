"use strict";

var app = angular.module("DeepinHelp", []);
app.filter("keyword", function() {
    var _PLACEHOLDER_RESULT = [];
    var lastTerm = null;
    var cached = null;
    return function(indices, keyword) {
        var result = [];
        if (!keyword) {
            return _PLACEHOLDER_RESULT;
        }
        if ((keyword === lastTerm) && cached) {
            return cached;
        }
        var lowerKeyword = keyword.toLowerCase();
        var lowerText;

        var object;
        var header;
        var texts;
        var text;
        nextHeader: for (var m in indices) {
            object = indices[m];
            header = object.header;
            texts = object.texts;
            for (text of texts) {
                lowerText = text.toLowerCase();
                if (lowerText.indexOf(lowerKeyword) >= 0) {
                    result.push({
                        header: header,
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
