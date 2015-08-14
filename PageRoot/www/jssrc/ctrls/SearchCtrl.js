"use strict";

let {
    dedupKeywords,
    sortKeywordsByLength,
    sortSearchResults,
} = require("../search");

angular.module("DManual")
    .controller("SearchCtrl", function($scope, GSynonym, $log,
                                       $filter, AdapterService, MarkdownService, localeService) {
        let _indices = [];
        $scope.searchResults = [];
        $scope.returnPageview = function() {
            $scope.isSearchMode = false;
        };

        let onMarkdownProcessed = function(event) {
            _indices = MarkdownService.getIndices();
        };
        $scope.$on("MarkdownProcessed", onMarkdownProcessed);
        if (MarkdownService.isInitialized()) {
            onMarkdownProcessed();
        }

        $scope.$on("searchTermChanged", function(event, value) {
            let keywords = [].concat(GSynonym.lookup(value));
            if (value) {
                // simple split by space
                keywords = keywords.concat(value.split(" "));

                // use word-cutting provided by the shell
                keywords = keywords.concat(AdapterService.getWordCutting(value));
            }
            keywords = dedupKeywords(keywords);
            keywords = sortKeywordsByLength(keywords);

            // apply filter
            const highLighted = $filter("filterHighlight")(_indices, value, keywords);

            // make header matches to go first
            const results = sortSearchResults(keywords, highLighted);

            $scope.searchResults = results;
        });

        $scope.openWiki = () => {
            let currentUiLang = localeService.getLocale();
            // wiki supports ?language=en, but not en_us
            // so make the language code short
            if (currentUiLang.includes("-")) {
                currentUiLang = currentUiLang.substr(0, currentUiLang.indexOf("-"));
            }
            if (currentUiLang.includes("_")) {
                currentUiLang = currentUiLang.substr(0, currentUiLang.indexOf("_"));
            }
            AdapterService.openExternalBrowser(`http://wiki.deepin.org/?language=${currentUiLang}`);
        }
});
