"use strict";

let splitSentences = function(text, delimiter) {
    // Split a paragraph to an array of sentences
    let sentences = [];

    let parts = text.split(delimiter);

    parts = parts.filter(function(part) {
        return part.trim();
    }, parts);

    sentences = sentences.concat(parts.map(function(part) {
        return part.trim() + delimiter;
    }));

    return sentences;
};

let dedupKeywords = function(arr) {
    // return a copy of array, without duplicates
    let result = [];
    let kwSet = new Set();

    arr.map(function(keyword) {
        let lowerKeyword = keyword.toLocaleLowerCase();
        if (!kwSet.has(lowerKeyword)) {
            kwSet.add(lowerKeyword);
            result.push(keyword);
        }
    });
    return result;
};

if (typeof exports !== "undefined") {
    exports.dedupKeywords = dedupKeywords;
    exports.splitSentences = splitSentences;
}
