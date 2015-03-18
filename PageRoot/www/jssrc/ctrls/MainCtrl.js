"use strict";
var jumpTo = function(anchor) {
    var body = document.getElementsByTagName("body")[0];
    body = angular.element(body);
    var contentWin = document.getElementById("Content").contentWindow;
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

var mainCtrl = angular.module("DManual")
    .controller("MainCtrl", function($scope, $log, $sce, $window) {
        // trust the following HTML, since it is published by us.
        $scope.isOverview = true;

        $scope.appName = "UnnamedApp";

        loadMarkdown("./manual/manual_zhCN.md", function(error, md) {
            if (!error) {
                var result = parseMarkdown(md);
                var html = result.html;
                var parsed = result.parsed;

                $scope.navigationItems = parsed.items;
                $scope.appInfo = parsed.appInfo;
                $scope.$broadcast("indicesSet", parsed.indices);
                setTimeout(function() {
                    // wait for SearchBoxCtrl to startup
                    $scope.$broadcast("headersSet", parsed.headers);
                }, 100);
                var base = "<base href='http://localhost:63342/deepin-user-manual/PageRoot/www/manual/'>" +
                    "<link rel='stylesheet' href='../style/reset.css' />" +
                    "<link rel='stylesheet' href='../style/content.css' />";
                $scope.htmlOutput = $sce.trustAsHtml(base + html);
            } else {
                console.error(error);
            }
        });

        $scope.jumpTo = function(anchor) {
            var body = document.getElementsByTagName("body")[0];
            body = angular.element(body);
            var contentWin = document.getElementById("Content").contentWindow;
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

        var win = angular.element($window);
        var updateOuterFrame = function() {
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
