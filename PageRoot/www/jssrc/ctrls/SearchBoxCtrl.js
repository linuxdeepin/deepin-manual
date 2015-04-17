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
                $log.log("Search show");
                $scope.searchBoxVisible = true;
                document.querySelector("#SearchInput").focus();
            });

            $scope.$on("hideSearchBox", function(){
                $log.log("Search hide");
                $scope.searchBoxVisible = false;
                document.querySelector("#SearchInput").blur();
            });

            $scope.showSearch = function(){
                $scope.searchBoxVisible = true;
            };
            $scope.hideSearch = function(){
                $scope.searchBoxVisible = false;
            };

            $window.addEventListener("IFrameShowEventProxy", function() {
                $scope.$emit("showSearchBox");
            });
            $window.addEventListener("searchBoxHideEvent", function() {
                if($scope.searchBoxVisible){
                    $scope.$emit("hideSearchBox");
                    $scope.$apply();
                }
            });
            // $scope.$watch("searchTerm", function(newValue, oldValue){
            //     $rootScope.$broadcast("searchTermChanged", newValue);
            // });
            $scope.$on("headersSet", function(event, value) {
                $log.log("Headers changed to", value);
                $scope.headers = value;
            });
       });
