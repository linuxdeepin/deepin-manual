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
            $scope.currentIndex = -1;

            Object.defineProperty($scope, "completionList", {
                get: () => [].slice.call(document.querySelector("#Suggestions").children)
            });

            $scope.completionUp = function(){
                if($scope.currentIndex == -1) {
                    $scope.currentIndex = $scope.completionList.length - 1;
                } else {
                    $scope.currentIndex--;
                }
            }

            $scope.completionDown = function(){
                if($scope.currentIndex == $scope.completionList.length - 1) {
                    $scope.currentIndex = -1;
                } else {
                    $scope.currentIndex++;
                }
            }

            $scope.doSearch = function($innerScope, $event){
                if ($event.keyCode === Keyboard.ENTER) {
                    $event.preventDefault();
                    if($scope.currentIndex == -1){
                        $rootScope.$broadcast("searchTermChanged", $scope.searchTerm);
                    } else {
                        window.jumpTo($scope.completionValue);
                    }
                    document.querySelector('#SearchInput').blur();
                }
                if ($event.keyCode === Keyboard.KEY_UP) {
                    $event.preventDefault();
                    $scope.completionUp();
                    angular.element($scope.completionList).removeClass('active');
                    angular.element($scope.completionList[$scope.currentIndex]).addClass('active')
                    if($scope.currentIndex !== -1) {
                        $scope.completionValue = $scope.completionList[$scope.currentIndex].children[0].title
                    }
                } else if ($event.keyCode === Keyboard.KEY_DOWN) {
                    $event.preventDefault();
                    $scope.completionDown();
                    angular.element($scope.completionList).removeClass('active');
                    angular.element($scope.completionList[$scope.currentIndex]).addClass('active')
                    if($scope.currentIndex !== -1) {
                        $scope.completionValue = $scope.completionList[$scope.currentIndex].children[0].title
                    }
                } else {
                    $scope.currentIndex = -1;
                }
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
                    let resultList = $scope.headers.map(function(text) {
                        let found = false;
                        let highlighted = text.replace(pattern, function(word) {
                            found = true;
                            return `<span class="highlight">${word}</span>`;
                        });
                        if (found) {
                            return `<li><a href="javascript:jumpTo('${text}');" title="${text}">${highlighted}</a></li>`;
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
