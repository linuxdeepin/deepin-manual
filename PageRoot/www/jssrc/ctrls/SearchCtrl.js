"use strict";

let {
    dedupKeywords,
    sortKeywordsByLength,
    sortSearchResults,
} = require("../search");

angular.module("DManual")
    .controller("SearchCtrl", function($scope, GSynonym, $log,
                                       $filter, AdapterService) {
        let _indices = [];
        $scope.searchResults = [];

        $scope.$on("indicesSet", function(event, value) {
            _indices = value;
        });
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
            const highLighted = $filter("filterHighlight")(_indices, value, keywords)

            // make header matches to go first
            const results = sortSearchResults(keywords, highLighted);

            $scope.searchResults = results;
        });
        $scope.$on("manualSearchFinished", function(value){
            [].slice.call(document.querySelectorAll('.match-text')).map(function(parNode){
                let height = parseInt(window.getComputedStyle(parNode).height),
                    lineHeight = parseInt("20px"),
                    limit = lineHeight * 2;
                if(height > limit) {
                    parNode.style.height = limit + "px";
                } else {
                    parNode.querySelector(".ellipsis").remove();
                }
            });
        });
});
