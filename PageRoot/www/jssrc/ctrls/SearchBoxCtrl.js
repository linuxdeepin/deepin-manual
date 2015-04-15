"use strict";

angular.module("DManual")
       .controller("SearchBoxCtrl", function($scope, $rootScope, $animate, $timeout, $log, $window) {
            $scope.headers = [];
            $scope.searchBoxVisible = false;

            $scope.doSearch = function($innerScope, $event){
                if ($event.keyCode == 13) {
                    $rootScope.$broadcast("searchTermChanged", $scope.searchTerm);
                    return true;
                }
                return false;
            };

            $scope.$on("showSearchBox", function(){
                $log.log("Search");
                $scope.searchBoxVisible = true;
                document.querySelector("#SearchInput").focus();
            });

            $scope.showSearch = function(){
                $scope.searchBoxVisible = true;
            };
            $scope.hideSearch = function(){
                $scope.searchBoxVisible = false;
            };

            $window.addEventListener("IFrameShowEventProxy", function() {
                $log.log("proxy..");
                $scope.$emit("showSearchBox");
            });
            // $scope.$watch("searchTerm", function(newValue, oldValue){
            //     $rootScope.$broadcast("searchTermChanged", newValue);
            // });
            $scope.$on("headersSet", function(event, value) {
                $log.log("Headers changed to", value);
                $scope.headers = value;
            });
       });
