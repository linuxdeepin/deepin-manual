"use strict";

let {
    processMarkdown,
} = require("../renderer");

let {
    getContentStylePath,
    getScriptPath,
} = require("../utils");

let app = angular.module("DManual");

app.controller("MainCtrl", function($scope, $rootScope, $log, $sce, $window, $timeout,
                                    hotkeys, GInput, GSynonym, AdapterService) {
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

        $scope.$watch("isPageview", function(value){
            let ev = new CustomEvent("resizeSidebar");
            $window.dispatchEvent(ev);
        });

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
            GInput.load(`${markdownDir}/index.md`).then(function(mdText) {
                $log.log("Markdown::load OK");
                let contentIFrame = document.querySelector("#Content");
                let result = processMarkdown(mdText);
                let html = result.html;
                let parsed = result.parsed;
                angular.element(contentIFrame).bind('load', function(){
                    let doc = contentIFrame.contentDocument;
                    $scope.anchorsOffsetList = [].slice.call(doc.querySelectorAll('h2,h3'))
                        .map(function(v){
                            return v.offsetTop
                    });
                    $scope.navigations = [].slice.call(
                            document.querySelector('#SideNavigationItems')
                                    .querySelectorAll('.level2,.level3'));
                });
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
                    <script src="${scriptPath}/mousetrap.js"></script>
                    <script>
                    'use strict';
                    let disallow = function(event) {
                        event.preventDefault();
                        return false;
                    };
                    let emitEvent = function(eventName, eventMsg) {
                        let e = new CustomEvent(eventName, {detail: eventMsg});
                        window.parent.dispatchEvent(e);
                    };
                    window.onload = function() {
                        let body = document.body;
                        body.addEventListener("dragenter", disallow);
                        body.addEventListener("dragover", disallow);
                        body.addEventListener("dragend", disallow);
                        body.addEventListener("dragleave", disallow);
                        body.addEventListener("drop", disallow);
                    };
                    Mousetrap.bind('ctrl+f', function() {
                        emitEvent("IFrameShowEventProxy");
                    });
                    Mousetrap.bind('esc', function() {
                        emitEvent("searchBoxHideEvent");
                    });
                    ['mousewheel', 'click'].map(function(eventName){
                        document.addEventListener(eventName, function(){
                            emitEvent("searchBoxHideEvent");
                        });
                    });
                    document.addEventListener('mousewheel', function(e){
                        emitEvent("navigationRelocateEvent", {offset: window.scrollY});
                    });
                    </script>
                    <link rel='stylesheet' href='${stylePath}/reset.css' />
                    <link rel='stylesheet' href='${stylePath}/content.css' />`;
                $scope.htmlOutput = $sce.trustAsHtml(base + html);
            }, function(error) {
                $log.error(`Markdown::load failed: ${error}`);
            });
        };
        let markdownDir = AdapterService.markdownDir();
        if (markdownDir) {
            loadMarkdown(null, markdownDir);
        }

        $scope.jumpTo = function(anchor) {
            let body = angular.element($window.document.body);
            let contentWin = document.getElementById("Content").contentWindow;
            if (anchor) {
                $scope.isOverview = false;
                $scope.isPageview = true;
                // not using `ng-class` here, trigger reflow
                body.removeClass('overview-mode');
                body.addClass('pageview-mode');
                $timeout(function() {
                    contentWin.location.hash = anchor;
                    $scope.$emit("navigationRelocate", contentWin.scrollY);
                }, 0);
            } else {
                $scope.isOverview = true;
                $scope.isPageview = false;
                // same as above
                body.removeClass('pageview-mode');
                body.addClass('overview-mode');
                $timeout(function() {
                    contentWin.location.hash = "";
                    $scope.$emit("navigationRelocate", contentWin.scrollY);
                }, 0);
            }
            $scope.isSearchmode = false;
        };
        $window.jumpTo = $scope.jumpTo;

        let updateOuterFrame = function() {
            $scope.outerFrameStyle = {
                height: $window.innerHeight + "px",
            };
        };
        updateOuterFrame();
        angular.element($window).bind("resize", function(event) {
            updateOuterFrame();
        });

        $scope.$on("searchTermChanged", function(event, value) {
            $scope.isSearchmode = value && value.length > 0;
        });

        $scope.$on("navigationBarToggled", function(event, value) {
            $rootScope.isCompactMode = value;
        });
    });
