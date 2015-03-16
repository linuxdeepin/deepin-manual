"use strict";

angular.module("DeepinHelp")
       .controller("SearchBoxCtrl", function($scope, $rootScope) {
            $scope.headers = [];
            $scope.$watch("searchTerm", function(newValue, oldValue){
                $rootScope.$broadcast("searchTermChanged", newValue);
            });
            $scope.$on("headersSet", function(event, value) {
                $scope.headers = value;
            });
       });