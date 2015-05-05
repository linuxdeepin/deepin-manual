"use strict";

let {
    processMarkdown,
} = require("../renderer");

let {
    getContentStylePath,
    getScriptPath,
} = require("../utils");

let app = angular.module("DManual");

app.controller("MainCtrl", function($scope, $rootScope, $log, $window, $timeout,
                                    hotkeys, GInput, GSynonym, AdapterService) {
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
    $scope.appInfo = {
        name: "Untitled",
    };
    $scope.anchorsOffsetList = [];
    $scope.navigations = [];

    // add hot keys
    hotkeys.add({
        combo: ['ctrl+f', '/'],
        description: 'show search box',
        callback: function() {
            if($scope.isPageview){
                $scope.$emit("showSearchBox", true);
            }
        }
    });
    hotkeys.add({
        combo: 'esc',
        description: 'hide search box',
        callback: function() {
            if($scope.isPageview){
                $scope.$emit("hideSearchBox", true);
            }
        }
    });

    // load markdown
    let loadMarkdown = function(event, markdownDir) {
        $log.log("Start to load markdown");
        GInput.load(`${markdownDir}/synonym.txt`).then(function(text) {
            let lines = text.split("\n");
            let wordsList = [];
            nextLine: for (let line of lines) {
                let words = line.split("|")
                                .filter(word => word.trim());
                if (words) {
                    wordsList.push(words);
                }
            }
            GSynonym.init(wordsList.filter(words => words.length > 0));
        }, function(error) {
            $log.warn(`Cannot load synonyms ${error}`);
            GSynonym.init([]);
        });

        let stylePath = getContentStylePath($window.location.href);
        let scriptPath = getScriptPath($window.location.href);
        GInput.load(`${markdownDir}/index.md`).then(function(mdText) {
            $log.log("Markdown::load OK");

            let result = processMarkdown(mdText);

            $timeout(function() {
                // TODO: use a better method to wait for ContentCtrl
                $scope.$broadcast("ContentHtmlReady", {
                    html: result.html,
                    stylePath: stylePath,
                    scriptPath: scriptPath,
                    markdownDir: markdownDir,
                });
            }, 100);

            let parsed = result.parsed;

            $scope.anchors = parsed.anchors;
            $scope.appInfo = parsed.appInfo;
            $scope.$broadcast("indicesSet", parsed.indices);
            $timeout(function() {
                // TODO: use a better method to wait for SearchBoxCtrl
                $scope.$broadcast("headersSet", parsed.headers);
            }, 100);

            $scope.appInfo.markdownDir = markdownDir;
        }, function(error) {
            $log.error(`Markdown::load failed: ${error}`);
        });
    };

    // Markdown reload
    let markdownDir = AdapterService.markdownDir();
    if (markdownDir) {
        loadMarkdown(null, markdownDir);
    }
    $scope.$on("markdownDirChanged", loadMarkdown);

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
