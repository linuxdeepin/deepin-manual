"use strict";

let {
    processMarkdown,
} = require("../renderer");

let {
    getContentStylePath,
    getScriptPath,
} = require("../utils");

angular.module("DManual")
    .controller("MainCtrl", function($scope, $rootScope, $log, $sce, $window, $timeout, hotkeys, GInput, GSynonym) {
        $scope.isOverview = true;
        let _isSearchmode = false;
        Object.defineProperty($scope, "isSearchmode", {
            get: () => _isSearchmode,
            set: (newValue) => {
                if (newValue !== _isSearchmode) {
                    if (newValue) {
                        $log.log("Enter search mode");
                        _isSearchmode = true;
                    } else {
                        $log.log("Leave search mode");
                        _isSearchmode = false;
                    }
                }
            }
        });
        $scope.appInfo = {
            name: "Unnamed",
        };

        hotkeys.add({
            combo: ['ctrl+f', '/'],
            description: 'show search box',
            callback: function() {
                if($scope.isPageview){
                    $rootScope.$broadcast("showSearchBox", true);
                }
            }
        });

        $scope.$watch("isPageview", function(value){
            let ev = new CustomEvent("resizeSidebar");
            $window.dispatchEvent(ev);
        });

        $scope.$on("setMarkdown", function(event, markdownDir) {
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
            GInput.load(`${markdownDir}/index.md`).then(function(mdText) {
                $log.log("Markdown::load OK");
                let result = processMarkdown(mdText);
                let html = result.html;
                let parsed = result.parsed;
                $scope.anchors = parsed.anchors;
                $scope.appInfo = parsed.appInfo;
                $scope.$broadcast("indicesSet", parsed.indices);
                $timeout(function() {
                    // wait for SearchBoxCtrl to startup
                    $scope.$broadcast("headersSet", parsed.headers);
                }, 100);
                let stylePath = getContentStylePath(location.href);
                let scriptPath = getScriptPath(location.href);
                $scope.appInfo.markdownDir = markdownDir;
                let base = `<base href='${markdownDir}/'>
                    <script src="${scriptPath}/iscroll.js"></script>
                    <script src="${scriptPath}/mousetrap.js"></script>
                    <script>
                    'use strict';
                    let disallow = function(event) {
                        event.preventDefault();
                        return false;
                    };
                    window.onload = function() {
                        let body = document.getElementsByTagName("body")[0];
                        body.addEventListener("dragenter", disallow);
                        body.addEventListener("dragover", disallow);
                        body.addEventListener("dragend", disallow);
                        body.addEventListener("dragleave", disallow);
                        body.addEventListener("drop", disallow);
                    }
                    Mousetrap.bind('ctrl+f', function() {
                        let e2 = new CustomEvent("IFrameShowEventProxy");
                        window.parent.dispatchEvent(e2);
                    });
                    </script>
                    <link rel='stylesheet' href='${stylePath}/reset.css' />
                    <link rel='stylesheet' href='${stylePath}/content.css' />`;
                $scope.htmlOutput = $sce.trustAsHtml(base + html);
            }, function(error) {
                $log.error(`Markdown::load failed: ${error}`);
            });
        });

        $scope.jumpTo = function(anchor) {
            let body = document.getElementsByTagName("body")[0];
            body = angular.element(body);
            let contentWin = document.getElementById("Content").contentWindow;
            if (anchor) {
                $scope.isOverview = false;
                $scope.isPageview = true;
                // not using `ng-class` here, trigger reflow
                body.removeClass('overview-mode');
                body.addClass('pageview-mode');
                $timeout(function() {
                    contentWin.location.hash = anchor;
                }, 0);
            } else {
                $scope.isOverview = true;
                $scope.isPageview = false;
                // same as above
                body.removeClass('pageview-mode');
                body.addClass('overview-mode');
                $timeout(function() {
                    contentWin.location.hash = "";
                }, 0);
            }
            $scope.isSearchmode = false;
        };
        $window.jumpTo = $scope.jumpTo;

        let win = angular.element($window);
        let updateOuterFrame = function() {
            $scope.outerFrameStyle = {
                height: $window.innerHeight + "px"
            };
        };
        updateOuterFrame();
        win.bind("resize", function(event) {
            updateOuterFrame();
        });

        $scope.$on("searchTermChanged", function(event, value) {
            $scope.isSearchmode = value && value.length > 0;
        });

        $scope.$on("navigationBarToggled", function(event, value) {
            $rootScope.isCompactMode = value;
        });
    });
