"use strict";

angular.module("DManual").controller("SearchCtrl", function($scope, GSynonym, $log, $filter) {
    let _indices = [];
    $scope.searchResults = [];

    $scope.$on("indicesSet", function(event, value) {
        _indices = value;
    });
    $scope.$on("searchTermChanged", function(event, value) {
        let keywords = [].concat(GSynonym.lookup(value));
        if (value) {
            keywords = keywords.concat(value.split(" "));
        }
        $scope.searchResults = $filter("filterHighlight")
                (_indices, value, keywords);
    });
});
