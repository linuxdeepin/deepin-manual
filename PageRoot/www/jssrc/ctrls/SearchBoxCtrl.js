"use strict";

let {
    normalizeAnchorName,
} = require("../renderer");

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
        $scope.suggestions = [];
        $scope.disableSuggestions = false;

        let onMarkdownProcessed = function(event) {
            let headers = MarkdownService.getHeaders();
            $log.log("Headers changed to", headers);
            $scope.headers = headers;
        };
        $scope.$on("MarkdownProcessed", onMarkdownProcessed);
        if (MarkdownService.isInitialized()) {
            onMarkdownProcessed();
        }

        $scope.completionUp = function() {
            $scope.currentIndex--;
            if ($scope.currentIndex < 0) {
                $scope.currentIndex = $scope.suggestions.length;
            }
        };
        $scope.completionDown = function() {
            $scope.currentIndex++;
            if ($scope.currentIndex > $scope.suggestions.length) {
                $scope.currentIndex = 0;
            }
        };

        $scope.doFullTextSearch = function(searchTerm) {
            if (searchTerm.trim()) {
                $rootScope.$broadcast("searchTermChanged", searchTerm);
            }
            $scope.disableSuggestions = true;
        };

        $scope.onKeydown = function($innerScope, $event) {
            if ($event.keyCode === Keyboard.ENTER) {
                $event.preventDefault();
                if (!$scope.searchTerm.trim()) {
                    return;
                }
                if ($scope.currentIndex === -1 || $scope.currentIndex === $scope.suggestions.length) {
                    $scope.doFullTextSearch($scope.searchTerm);
                } else {
                    $scope.doJumpToSuggestion($scope.suggestions[$scope.currentIndex].anchorId);
                }
                document.querySelector('#SearchInput').blur();
            } else if ($event.keyCode === Keyboard.KEY_UP) {
                $event.preventDefault();
                $scope.completionUp();
            } else if ($event.keyCode === Keyboard.KEY_DOWN) {
                $event.preventDefault();
                $scope.completionDown();
            } else if ($event.keyCode === Keyboard.ESCAPE) {
                if (!$scope.searchTerm.trim()) {
                    $scope.$emit("hideSearchBox", "searchinput-shortcut");
                }
            } else {
                $scope.currentIndex = -1;
            }
        };

        $scope.doJumpToSuggestion = function(anchorId) {
            $scope.jumpTo(anchorId);
            $scope.disableSuggestions = true;
            $scope.searchInputVisible = [false, "jumpto-suggestion"];
        };

        // search suggestions
        $scope.$watch("searchTerm", function(value) {
            let suggestions = [];

            if (value) {
                let pattern = new RegExp("(?:" +
                    value.replace(/([.*+?^=!:${}()|\[\]\/\\])/g, '\\$1')
                    + ")", 'gi');
                $scope.headers.map(function(text) {
                    let found = false;
                    let highlighted = text.replace(pattern, function(word) {
                        found = true;
                        return `<span class="highlight">${word}</span>`;
                    });
                    if (found) {
                        suggestions.push({
                            anchorId: normalizeAnchorName(text),
                            anchorHtmlText: $sce.trustAsHtml(highlighted),
                        });
                    }
                });
            }
            $scope.suggestions = suggestions;
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
            $scope.$apply();
        });

        $window.addEventListener("searchBoxHideEvent", function(event) {
            // activate from iframe#Content
            $scope.searchInputVisible = [false, event.detail.reason];
            $scope.$apply();
        });

        let unboundMatches = document.body.matches
                          || document.body.webkitMatchesSelector
                          || null;
        if (!unboundMatches) {
            throw new Error("Cannot Match Selector");
        }
        $window.addEventListener("mousedown", function(event) {
            // determine if the click-target is in the "search zone"
            let target = event.target;
            let inSearchBoxArea = false;
            do {
                if (target === null || target instanceof HTMLDocument) {
                    break;
                }
                if (unboundMatches.call(target, "#SearchBox")) {
                    inSearchBoxArea = true;
                    break;
                }
            } while (target = target.parentNode);

            if (!inSearchBoxArea) {
                $scope.searchInputVisible = [false, "rootframe-mousedown-elsewhere"];
            }
            $scope.$apply();
        }, true);

        $scope.$watch("isPageview", function(isPageview) {
            if (isPageview && (!$scope.isSearchMode)) {
                if (AdapterService.isFirstRun()) {
                    // to avoid animation
                    _searchInput.classList.add("slidedown");
                    $log.log("Show SearchBox: initial");
                    let hideTimeout = 3000;
                    _showHidePromise = $timeout(function () {
                        $scope.searchInputVisible = [false, `timeout after ${hideTimeout}ms`];
                        _showHidePromise = null;
                    }, hideTimeout);
                } else {
                    _searchInputVisible = false;
                }
            }
        });

        let _mouseInSearchBox = false;
        $scope.onSearchBoxMouseEnter = function() {
            _mouseInSearchBox = true;
            $scope.searchInputVisible = [true, "searchbox-mouseenter"];
        };
        $scope.onSearchBoxMouseLeave = function() {
            _mouseInSearchBox = false;
            // It would become very hard to use the SearchInput hides when mouse moves away,
            // so comment the following out.

            //if ($window.document.activeElement !== _searchInput) {
            //    $scope.searchInputVisible = [false, "searchbox-mouseleave"];
            //}
        };
        $scope.onSearchInputFocus = function() {
            $scope.disableSuggestions = false;
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
