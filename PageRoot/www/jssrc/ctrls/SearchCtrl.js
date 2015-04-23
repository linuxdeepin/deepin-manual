"use strict";

let {
    dedupKeywords,
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
            $scope.searchResults = $filter("filterHighlight")
                    (_indices, value, keywords);
        });
});
