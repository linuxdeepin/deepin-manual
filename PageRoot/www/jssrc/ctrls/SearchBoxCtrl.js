"use strict";

let Keyboard = {
    ENTER: 13,
    KEY_UP: 38,
    KEY_DOWN: 40,
    ESCAPE: 27,
};

angular.module("DManual").controller("SearchBoxCtrl",
    function($scope, $rootScope, $animate, $timeout,
             $log, $sce, $window, AdapterService, MarkdownService) {
        // Auto-complete
        $scope.headers = [];
        $scope.currentIndex = -1;
        $scope.searchTerm = "";

        let onMarkdownProcessed = function(event) {
            let headers = MarkdownService.getHeaders();
            $log.log("Headers changed to", headers);
            $scope.headers = headers;
        };
        $scope.$on("MarkdownProcessed", onMarkdownProcessed);
        if (MarkdownService.isInitialized()) {
            onMarkdownProcessed();
        }

        Object.defineProperty($scope, "completionList", {
            get: () => [].slice.call(document.querySelector("#Suggestions").children)
        });
        $scope.completionUp = function(){
            if($scope.currentIndex == -1) {
                $scope.currentIndex = $scope.completionList.length - 1;
            } else {
                $scope.currentIndex--;
            }
        };
        $scope.completionDown = function(){
            if($scope.currentIndex == $scope.completionList.length - 1) {
                $scope.currentIndex = -1;
            } else {
                $scope.currentIndex++;
            }
        };

        // do search
        $scope.doSearch = function($innerScope, $event){
            if ($event.keyCode === Keyboard.ENTER) {
                $event.preventDefault();
                if (!$scope.searchTerm.trim()) {
                    return;
                }
                if ($scope.currentIndex == -1) {
                    $rootScope.$broadcast("searchTermChanged", $scope.searchTerm);
                    let body = angular.element(document.body);
                    body.addClass('pageview-mode');
                    body.removeClass('overview-mode');
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
                if ($scope.currentIndex !== -1) {
                    $scope.completionValue = $scope.completionList[$scope.currentIndex].children[0].title
                }
            } else if ($event.keyCode === Keyboard.KEY_DOWN) {
                $event.preventDefault();
                $scope.completionDown();
                angular.element($scope.completionList).removeClass('active');
                angular.element($scope.completionList[$scope.currentIndex]).addClass('active')
                if ($scope.currentIndex !== -1) {
                    $scope.completionValue = $scope.completionList[$scope.currentIndex].children[0].title
                }
            } else if ($event.keyCode === Keyboard.ESCAPE) {
                if (!$scope.searchTerm.trim()) {
                    $scope.$emit("hideSearchBox", "searchinput-shortcut");
                }
            } else {
                $scope.currentIndex = -1;
            }
        };

        // search suggestions
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

        // show-hide logic
        let _showHidePromise = null;
        let _searchInput = document.getElementById("SearchInput");
        let _searchInputVisible = true; // if searchInput is visible in page view
        Object.defineProperty($scope, "searchInputVisible", {
            get: () => _searchInputVisible,
            set: function(newValue) {
                if (!$scope.isPageview || $scope.isSearchMode) {
                    $log.warn("The attempt to change searchInputVisible is aborted: Wrong view.");
                    return;
                }
                let [show, reason] = newValue;
                if (_showHidePromise) {
                    $timeout.cancel(_showHidePromise);
                    _showHidePromise = null;
                }
                if (show === _searchInputVisible) {
                    return;
                }
                if (show) {
                    $log.log(`Show SearchBox: ${reason}`);
                    $animate.addClass(_searchInput, "slidedown");
                    if (reason === "rootframe-shortcut" || reason === "contentframe-shortcut") {
                        _searchInput.focus();
                    }
                    _searchInputVisible = true;
                } else {
                    $log.log(`Hide SearchBox: ${reason}`);
                    $animate.removeClass(_searchInput, "slidedown");
                    _searchInput.blur();
                    _searchInputVisible = false;
                }
                try {
                    // sometimes multiple applies will occur
                    $scope.$digest();
                } catch (e) {};
            },
        });

        $scope.$on("showSearchBox", function(event, reason) {
            // activate from everything other than iframe#Content
            $scope.searchInputVisible = [true, reason];
        });

        $scope.$on("hideSearchBox", function(event, reason) {
            // activate from everything other than iframe#Content
            $scope.searchInputVisible = [false, reason];
        });

        $window.addEventListener("IFrameShowEventProxy", function(event) {
            // activate from iframe#Content
            $scope.searchInputVisible = [true, event.detail.reason];
        });

        $window.addEventListener("searchBoxHideEvent", function(event) {
            // activate from iframe#Content
            $scope.searchInputVisible = [false, event.detail.reason];
        });
        $window.addEventListener("mousedown", function(event) {
            // determine if the click-target is in the "search zone"
            let target = event.target;
            let inSearchBoxArea = false;
            do {
                if (target === null || target instanceof HTMLDocument) {
                    break;
                }
                let unboundMatches = target.matches || target.webkitMatchesSelector || null;
                if (!unboundMatches) {
                    throw new Error("Cannot Match Selector");
                }
                if (unboundMatches.call(target, "#SearchBox")) {
                    inSearchBoxArea = true;
                    break;
                }
            } while (target = target.parentNode);

            if (!inSearchBoxArea) {
                $scope.searchInputVisible = [false, "rootframe-mousedown-elsewhere"];
            }
        }, true);

        $scope.$watch("isPageview", function(isPageview) {
            if (isPageview && (!$scope.isSearchMode)) {
                // to avoid animation
                _searchInput.classList.add("slidedown");
                $log.log("Show SearchBox: initial");

                let hideTimeout;
                if (AdapterService.isFirstRun()) {
                    hideTimeout = 3000;
                } else {
                    hideTimeout = 1500;
                }
                _showHidePromise = $timeout(function() {
                    $scope.searchInputVisible = [false, `timeout after ${hideTimeout}ms`];
                    _showHidePromise = null;
                }, hideTimeout);
            }
        });

        let _mouseInSearchBox = false;
        $scope.onSearchBoxMouseEnter = function() {
            _mouseInSearchBox = true;
            $scope.searchInputVisible = [true, "searchbox-mouseenter"];
        };
        $scope.onSearchBoxMouseLeave = function() {
            _mouseInSearchBox = false;
        };
        $scope.onSearchInputBlur = function() {
            if (!$scope.isPageview) {
                return;
            }
            if (_mouseInSearchBox) {
                return;
            }
            if ($scope.isSearchMode) {
                return;
            }
            $scope.searchInputVisible = [false, "searchinput-blur"];
        };
});
