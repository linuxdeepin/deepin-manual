"use strict";

angular.module("DeepinHelp").controller("SearchCtrl", function($scope, $rootScope) {
    $scope.searchTerm = null;
    $scope.matches = [];

    $scope.$on("indicesSet", function(event, value) {
        $scope.matches = value;
    });
    $rootScope.$on("searchTermChanged", function(event, value) {
        $scope.searchTerm = value;
    });
});
