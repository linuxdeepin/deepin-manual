"use strict";

let app = angular.module("DManual");

app.controller("MainCtrl", function($scope, $rootScope, $log, $window,
                                    hotkeys, MarkdownService) {
    // local states and flags
    $scope.isOverview = true;
    $scope.isPageview = !$scope.isOverview;
    let _isSearchMode = false;
    Object.defineProperty($scope, "isSearchMode", {
        get: () => _isSearchMode,
        set: (newValue) => {
            if (newValue !== _isSearchMode) {
                if (newValue) {
                    $log.log("Enter search mode");
                    _isSearchMode = true;
                } else {
                    $log.log("Leave search mode");
                    _isSearchMode = false;
                }
            }
        }
    });

    // add hot keys
    hotkeys.add({
        combo: ['ctrl+f', '/'],
        description: 'show search box',
        callback: function() {
            if ($scope.isPageview) {
                $scope.$broadcast("showSearchBox", "rootframe-shortcut");
            }
        }
    });
    hotkeys.add({
        combo: 'esc',
        description: 'hide search box',
        callback: function() {
            if ($scope.isPageview) {
                $scope.$broadcast("hideSearchBox", "rootframe-shortcut");
            }
        }
    });

    $scope.anchors = [];
    $scope.appInfo = {
        name: "Untitled",
    };
    $scope.anchorsOffsetList = [];
    $scope.navigations = [];

    let onMarkdownProcessed = function(event) {
        $scope.anchors = MarkdownService.getAnchors();
        $scope.appInfo = MarkdownService.getAppInfo();
    };
    $scope.$on("MarkdownProcessed", onMarkdownProcessed);
    if (MarkdownService.isInitialized()) {
        onMarkdownProcessed();
    }

    // jumpTo
    $scope.jumpTo = function(anchor) {
        // Because
        //   1) jumpTo needs to be accessed from all over the app;
        //   2) jumpTo needs to modify isPageview and other global
        // flags
        // it needs to be in MainCtrl instead of ContentCtrl.
        $scope.isPageview = !!anchor;
        $scope.isOverview = !$scope.isPageview;
        $scope.isSearchMode = false;
        // the real implementation is in ContentCtrl
        $scope.$broadcast("jumpTo", anchor);
    };
    $window.jumpTo = $scope.jumpTo;


    // Events from other parts of the program
    $scope.$on("searchTermChanged", function(event, value) {
        $scope.isSearchMode = value && value.length > 0;
    });

    $scope.$on("navigationBarToggled", function(event, value) {
        $rootScope.isCompactMode = value;
    });
    $scope.$on("ContentHtmlLoaded", function(event) {
        let contentIFrame = document.querySelector("#Content");
        let doc = contentIFrame.contentDocument;
        $scope.anchorsOffsetList = [].slice.call(doc.querySelectorAll('h2,h3'))
            .map((v) => v.offsetTop);

        $scope.navigations = [].slice.call(
            document.querySelector('#SideNavigationItems')
                    .querySelectorAll('.level2,.level3'));
    });
});
