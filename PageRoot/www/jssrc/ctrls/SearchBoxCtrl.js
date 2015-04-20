"use strict";

let Keyboard = {
    ENTER: 13,
    KEY_UP: 38,
    KEY_DOWN: 40
};

angular.module("DManual")
       .controller("SearchBoxCtrl", function($scope, $rootScope, $animate, $timeout, $log, $sce, $window) {
            $scope.headers = [];
            $scope.searchBoxVisible = false;

            $scope.doSearch = function($innerScope, $event){
                if ($event.keyCode === Keyboard.ENTER) {
                    $rootScope.$broadcast("searchTermChanged", $scope.searchTerm);
                    return true;
                } else if ($event.keyCode === Keyboard.KEY_UP) {
                    // TODO search cursor up
                } else if ($event.keyCode === Keyboard.KEY_DOWN) {
                    // TODO search cursor down
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
            $scope.$watch("searchTerm", function(value){
                if (value) {
                    let pattern = new RegExp("(?:" +
                        value.replace(/([.*+?^=!:${}()|\[\]\/\\])/g, '\\$1')
                    + ")", 'gi');
                    let headersList = $scope.headers;
                    let resultList = headersList.map(function(text) {
                        let found = false;
                        let highlighted = text.replace(pattern, function(word) {
                            found = true;
                            return `<span class="highlight">${word}</span>`;
                        });
                        if (found) {
                            return `<li><a href="javascript:jumpTo('${text}');">${highlighted}</a></li>`;
                        } else {
                            return false;
                        }
                    });
                    $scope.suggestions = $sce.trustAsHtml(resultList.filter(function(li) {
                        return !!li;
                    }).join("\n"));
                } else {
                    $scope.suggestions = "";
                }
            });
            $scope.$on("headersSet", function(event, value) {
                $log.log("Headers changed to", value);
                $scope.headers = value;
            });
       });
