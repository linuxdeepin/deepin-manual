"use strict";

angular.module("DManual").controller("SearchCtrl", function($scope, $rootScope, GSynonym, $log) {
    $scope.searchTerm = null;
    $scope.indices = [];

    // TODO: use real synonyms
    GSynonym.init([
        ["PC", "Personal Computer", "Computer"],
        ["Turn on", "Power up"],
        ["Turn off", "Power off", "Power down", "Shutdown", "Shut down"],
    ]);
    let tmp = GSynonym.lookup("Click me to turn off your computer");
    $log.log(`Got GSynonym.lookup: ${tmp}`);

    $scope.$on("indicesSet", function(event, value) {
        $scope.indices = value;
    });
    $rootScope.$on("searchTermChanged", function(event, value) {
        $scope.searchTerm = value;
    });
});
