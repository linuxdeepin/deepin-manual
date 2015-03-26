"use strict";

angular.module("DManual").controller("SearchCtrl", function($scope, $rootScope) {
    $scope.searchTerm = null;
    $scope.indices = [];

    $scope.$on("indicesSet", function(event, value) {
        $scope.indices = value;
    });
    $rootScope.$on("searchTermChanged", function(event, value) {
        $scope.searchTerm = value;
    });
});
