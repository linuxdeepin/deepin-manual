"use strict";

angular.module("DManual")
       .controller("SearchBoxCtrl", function($scope, $rootScope, $log) {
            $scope.headers = [];
            $scope.$watch("searchTerm", function(newValue, oldValue){
                $rootScope.$broadcast("searchTermChanged", newValue);
            });
            $scope.$on("headersSet", function(event, value) {
                $log.log("Headers changed to", value);
                $scope.headers = value;
            });
       });
