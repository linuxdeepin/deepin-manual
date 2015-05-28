"use strict";

let {
    bound,
} = require("../utils");

const READER_MAX_HEIGHT = 310;
const READER_MIN_HEIGHT = 100;
const READER_BORDER_VERTICAL = 1;
const READER_PADDING_VERTICAL = 24;
const ICON_HEIGHT = 20;
const ICON_WIDTH = 20;
const ICON_VERTICAL_DISTANCE = 10;
const TRIANGLE_HEIGHT = 7;
const TRIANGLE_WIDTH = 12;

angular.module("DManual").controller("ExternalReaderCtrl",
    function($scope, $log, $sce, $window, $rootScope, gettextCatalog,
             ExternalReaderService) {
    let iframe = $window.document.getElementById("ExternalReaderContent");
    let iconEle = null;
    let content = $window.document.getElementById("Content");
    let contentBody = null;
    let trianglesEle = $window.document.querySelector("span.triangles");
    let spaceholder = null;

    $scope.readerTop = "0";
    $scope.readerClasses = "";

    $scope.html = "";
    Object.defineProperty($scope, "shouldShowReader", {
        get: () => (!!$scope.html) && ($scope.isPageview) && (!$scope.isSearchMode),
    });

    $scope.$on("ExternalReaderContentReady", function(event, html) {
        iconEle.classList.add("active");
        trianglesEle.classList.add("active");
        $scope.html = $sce.trustAsHtml(html);
        iframe.contentWindow.focus();
    });

    let resetReaderVisually = function() {
        if (iconEle) {
            iconEle.classList.remove("active");
        }
        if (trianglesEle) {
            trianglesEle.classList.remove("active");
            trianglesEle.classList.remove("onTop");
            trianglesEle.classList.remove("onBottom");
        }
    };

    $scope.$on("ExternalReaderClose", function(event, reason) {
        $log.log(`ExternalReaderClose, reason: ${reason}`);
        resetReaderVisually();
        $scope.html = "";
        $scope.$apply();
    });

    $scope.externalRead = function(link, linkEle) {
        let {
            markdownDir,
            fromHeaderId,
            toHeaderId,
        } = ExternalReaderService.parseExternalLink(link);

        resetReaderVisually();

        // find the new icon
        iconEle = linkEle.querySelector("span.icon");

        let iconOffsetLeftToPage = iconEle.offsetLeft - spaceholder.offsetLeft;

        // determine reader on left/center/right
        let pageWidth = parseInt(content.contentWindow.getComputedStyle(spaceholder).width);
        let klass;

        if ((0 <= iconOffsetLeftToPage) && (iconOffsetLeftToPage < pageWidth / 3)) {
            klass = "onLeft";
        } else if ((pageWidth / 3 < iconOffsetLeftToPage) && (iconOffsetLeftToPage < pageWidth / 3 * 2)) {
            klass = "onCenter";
        } else {
            klass = "onRight";
        }

        // determine reader on top/bottom
        let iconOffsetTopToPage = iconEle.offsetTop - contentBody.scrollTop;
        let pageHeight = parseInt($window.getComputedStyle(content)["height"]);
        let onTop = (iconOffsetTopToPage + ICON_HEIGHT / 2) >= (pageHeight / 2);

        if (onTop) {
            klass += " onTop";
            trianglesEle.classList.add("onTop");
            trianglesEle.style.left = (iconEle.offsetLeft + ICON_WIDTH / 2) - TRIANGLE_WIDTH / 2 + TRIANGLE_WIDTH + "px";
            trianglesEle.style.top = iconOffsetTopToPage
                - (ICON_VERTICAL_DISTANCE - TRIANGLE_HEIGHT)
                - READER_BORDER_VERTICAL * 2 + "px";
        } else {
            klass += " onBottom";
            trianglesEle.classList.add("onBottom");
            trianglesEle.style.left = (iconEle.offsetLeft + ICON_WIDTH / 2) - TRIANGLE_WIDTH / 2 + "px";
            trianglesEle.style.top = iconOffsetTopToPage
                + (ICON_HEIGHT + ICON_VERTICAL_DISTANCE - TRIANGLE_HEIGHT)
                + READER_BORDER_VERTICAL * 2 + "px";
        }

        ExternalReaderService.loadExternalDManual(markdownDir, fromHeaderId, toHeaderId)
            .then(function(html) {
                $rootScope.$broadcast("ExternalReaderContentReady", html);
            }, function(error) {
                let TEXT_FAILED_TO_OPEN_EXTERNAL = gettextCatalog.getString("TEXT_FAILED_TO_OPEN_EXTERNAL");
                $rootScope.$broadcast("ExternalReaderContentReady",
                    `<div style='color: red'>${TEXT_FAILED_TO_OPEN_EXTERNAL}</div>`);
            });

        $scope.readerClasses = klass;
    };
    $window.externalRead = $scope.externalRead;

    angular.element(content).bind("load", function() {
        contentBody = content.contentDocument.body;
        spaceholder = content.contentDocument.querySelector("footer.__spaceholder");

        angular.element(iframe).bind("load", function() {
            // When the contentWindow loses focus, hide the external reader.
            angular.element(iframe.contentWindow).bind("blur", function() {
                $rootScope.$broadcast("ExternalReaderClose", "reader-blur");
            });

            let readerContentHeight = iframe.contentDocument.body.clientHeight;
            readerContentHeight = bound(
                READER_MIN_HEIGHT - (READER_PADDING_VERTICAL * 2 + READER_BORDER_VERTICAL * 2),
                readerContentHeight,
                READER_MAX_HEIGHT - (READER_PADDING_VERTICAL * 2 + READER_BORDER_VERTICAL * 2));

            let readerBoxHeight = readerContentHeight + (READER_PADDING_VERTICAL * 2 + READER_BORDER_VERTICAL * 2);

            let iconOffsetTopToPage = iconEle.offsetTop - contentBody.scrollTop;
            let readerTop;
            if ($scope.readerClasses.includes("onTop")) {
                readerTop = iconOffsetTopToPage - readerBoxHeight - ICON_VERTICAL_DISTANCE;
            } else {
                readerTop = iconOffsetTopToPage + ICON_HEIGHT + ICON_VERTICAL_DISTANCE;
            }
            $scope.readerHeight = readerContentHeight + "px";
            $scope.readerTop = `${readerTop}px`;
            $scope.$apply();
        });
    });

    angular.element($window).bind("contentScrollEvent", (event) => {
        $scope.$emit("ExternalReaderClose", "content-scrolled");
    });
});
