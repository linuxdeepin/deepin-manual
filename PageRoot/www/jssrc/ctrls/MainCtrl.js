"use strict";

let {
    loadMarkdown,
    parseMarkdown,
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
        body.removeClass("isSearchview");
        body.addClass("isPageview");
        setTimeout(function() {
            contentWin.location.hash = anchor;
        }, 0);
    } else {
        body.removeClass("isPageview");
        body.removeClass("isSearchview");
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
        $scope.appInfo = {
            appName: "UnnamedApp",
        };

        let mdUrl = "file:///home/xinkai/projects/deepin-user-manual/PageRoot/www/manual/manual_zhCN.md";
        loadMarkdown(mdUrl, function(error, payload) {
            if (!error) {
                let result = parseMarkdown(payload.markdown);
                let html = result.html;
                let parsed = result.parsed;

                let fileInfo = payload.fileInfo;

                $scope.navigationItems = parsed.items;
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
                    <link rel='stylesheet' href='${stylePath}/reset.css' />
                    <link rel='stylesheet' href='${stylePath}/content.css' />`;
                $scope.htmlOutput = $sce.trustAsHtml(base + html);
            } else {
                $log.error(error);
            }
        });

        $scope.jumpTo = function(anchor) {
            let body = document.getElementsByTagName("body")[0];
            body = angular.element(body);
            let contentWin = document.getElementById("Content").contentWindow;
            if (anchor) {
                $scope.isOverview = false;
                body.removeClass("isOverview");
                body.removeClass("isSearchview");
                body.addClass("isPageview");
                setTimeout(function() {
                    contentWin.location.hash = anchor;
                }, 0);
            } else {
                $scope.isOverview = true;
                body.removeClass("isPageview");
                body.removeClass("isSearchview");
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

    });
