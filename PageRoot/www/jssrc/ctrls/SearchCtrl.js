"use strict";

angular.module("DManual").controller("SearchCtrl", function($scope, GSynonym, $log) {
    $scope.searchTerm = null;
    $scope.indices = [];
    $scope.keywords = null;

    $scope.$on("indicesSet", function(event, value) {
        $scope.indices = value;
    });
    $scope.$on("searchTermChanged", function(event, value) {
        let keywords = [].concat(GSynonym.lookup(value));
        if (value) {
            keywords = keywords.concat(value.split(" "));
        }
        $scope.keywords = keywords;
        $scope.searchTerm = value;
    });
});
