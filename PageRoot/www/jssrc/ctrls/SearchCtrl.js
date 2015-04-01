"use strict";

angular.module("DManual").controller("SearchCtrl", function($scope, GSynonym, $log) {
    $scope.searchTerm = null;
    $scope.indices = [];
    $scope.keywords = null;

    // TODO: use real synonyms
    GSynonym.init([
        ["PC", "Personal Computer", "Computer"],
        ["Turn on", "Power up"],
        ["Turn off", "Power off", "Power down", "Shutdown", "Shut down"],
        ["电脑", "计算机", "PC"],
        ["深度", "Deepin"],
        ["开启计算机", "开机", "启动", "起动"],
        ["屏幕", "显示器"],
        ["深度帮助手册", "DManual", "DMan"],
    ]);

    $scope.$on("indicesSet", function(event, value) {
        $scope.indices = value;
    });
    $scope.$on("searchTermChanged", function(event, value) {
        $scope.keywords = [].concat(GSynonym.lookup(value)).concat([value]);
        $scope.searchTerm = value;
    });
});
