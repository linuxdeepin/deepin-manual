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
        let result = [];
        if (_wordMap === null) {
            console.warn("GSynonym not init yet.");
            return result;
        }
        if (typeof rawString !== "string") {
            return result;
        }
        console.time("GSynonym.lookup");
        rawString = rawString.toLocaleLowerCase();
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
