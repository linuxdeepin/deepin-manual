"use strict";

angular.module("General")
    .factory("GSynonym", function($log) {
    let _interface = {};
    let _synonymList = [];

    let _wordMap = null; // (word, synonymArray) pairs
    /**
     * Do word-cutting based on a list of synonym arrays.
     */
    _interface.init = function(synonymList) {
        $log.log("Loading synonyms", synonymList);
        // remember the synonymList
        _synonymList = synonymList;
        _wordMap = new Map();
        for (let synonymArray of _synonymList) {
            for (let synonym of synonymArray) {
                if (_wordMap.has(synonym)) {
                    $log.warn("synonym duplicates found:", synonym);
                } else {
                    // merge synonyms and warns about duplicates.
                    _wordMap.set(synonym, synonymArray);
                }
            }
        }
    };

    /**
     * Returns an array of keyword strings, which will be used in a full-text search.
     * @param rawString The raw input user has entered, not yet split.
     */
    _interface.lookup = function(rawString) {
        if ((!_wordMap) || _wordMap.size === 0) {
            throw new Error("GSynonym not init yet.");
        }
        console.time("GSynonym.lookup");
        rawString = rawString.toLocaleString();
        let result = [];
        for (let [key, synonymArray] of _wordMap.entries()) {
            if (rawString.indexOf(key.toLocaleLowerCase()) >= 0) {
                result = result.concat(synonymArray);
            }
        }
        console.timeEnd("GSynonym.lookup");
        return result;
    };

    return _interface;
});
