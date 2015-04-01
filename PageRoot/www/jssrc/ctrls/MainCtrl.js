"use strict";

let {
    loadMarkdown,
    processMarkdown,
} = require("../renderer");

let {
    getContentStylePath,
} = require("../utils");

let jumpTo = function(anchor) {
    let body = document.getElementsByTagName("body")[0];
    body = angular.element(body);
    let contentWin = document.getElementById("Content").contentWindow;
    if (anchor) {
        body.removeClass("isOverview");
        body.addClass("isPageview");
        setTimeout(function() {
            contentWin.location.hash = anchor;
        }, 0);
    } else {
        body.removeClass("isPageview");
        body.addClass("isOverview");
        setTimeout(function() {
            contentWin.location.hash = "";
        }, 0);
    }
};

if (typeof window !== "undefined") {
    window.jumpTo = jumpTo;
}

angular.module("DManual")
    .controller("MainCtrl", function($scope, $log, $sce, $window) {
        $scope.isOverview = true;
        $scope.isSearchmode = false;
        $scope.appInfo = {
            name: "Unnamed",
        };

        $scope.$on("setMarkdown", function(event, mdUrl) {
            loadMarkdown(mdUrl, function(error, payload) {
                if (!error) {
                    let result = processMarkdown(payload.markdown);
                    let html = result.html;
                    let parsed = result.parsed;

                    let fileInfo = payload.fileInfo;

                    $scope.anchors = parsed.anchors;
                    $scope.appInfo = parsed.appInfo;
                    $scope.$broadcast("indicesSet", parsed.indices);
                    setTimeout(function() {
                        // wait for SearchBoxCtrl to startup
                        $scope.$broadcast("headersSet", parsed.headers);
                    }, 100);
                    let stylePath = getContentStylePath(location.href);
                    let markdownDir = fileInfo.dir;
                    $scope.appInfo.markdownDir = markdownDir;
                    let base = `<base href='${markdownDir}/'>
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
                        </script>
                        <link rel='stylesheet' href='${stylePath}/reset.css' />
                        <link rel='stylesheet' href='${stylePath}/content.css' />`;
                    $scope.htmlOutput = $sce.trustAsHtml(base + html);

                    // TUL: Without this, the markdown won't actually be shown.
                    $scope.$apply();
                } else {
                    $log.error(error);
                }
            });
        });

        $scope.jumpTo = function(anchor) {
            let body = document.getElementsByTagName("body")[0];
            body = angular.element(body);
            let contentWin = document.getElementById("Content").contentWindow;
            if (anchor) {
                $scope.isOverview = false;
                $scope.isPageview = true;
                body.removeClass("isOverview");
                body.addClass("isPageview");
                setTimeout(function() {
                    contentWin.location.hash = anchor;
                }, 0);
            } else {
                $scope.isOverview = true;
                $scope.isPageview = false;
                body.removeClass("isPageview");
                body.addClass("isOverview");
                setTimeout(function() {
                    contentWin.location.hash = "";
                }, 0);
            }
        };

        let win = angular.element($window);
        let updateOuterFrame = function() {
            $scope.outerFrameStyle = {
                height: $window.innerHeight + "px"
            };
        };
        updateOuterFrame();
        win.bind("resize", function(event) {
            updateOuterFrame();
            $scope.$apply();
        });

        $scope.$on("searchTermChanged", function(event, value) {
            $log.log("searchTermChanged", value);
            if (value && value.length > 0) {
                $log.log("Enter search mode");
                $scope.isSearchmode = true;
            } else {
                $log.log("Leave search mode");
                $scope.isSearchmode = false;
            }
        });
    });
