"use strict";

let {
    bound,
} = require("../utils");

angular.module("DManual").controller("ExternalReaderCtrl",
    function($scope, $log, $sce, $window, $rootScope, gettext,
             ExternalReaderService) {
    let iframe = $window.document.getElementById("ExternalReaderContent");
    let iconEle = null;
    let content = $window.document.getElementById("Content");
    let contentBody = null;

    $scope.readerTop = "0";
    $scope.readerClasses = "";

    angular.element(content).bind("load", function() {
        contentBody = content.contentDocument.body;

        angular.element(iframe).bind("load", function() {
            angular.element(iframe.contentWindow).bind("blur", function() {
                $rootScope.$broadcast("ExternalReaderClose", "reader-blur");
            });
            let readerContentHeight = iframe.contentDocument.body.clientHeight;
            readerContentHeight = bound(100 - 48 /* marginTop + marginBottom */,
                readerContentHeight,
                310 - 48 /* marginTop + marginBottom */);
            $scope.readerHeight = readerContentHeight + "px";
            if ($scope.readerClasses.includes("onTop")) {
                $scope.readerTop = `${iconEle.offsetTop - contentBody.scrollTop - parseInt($scope.readerHeight) - 58}px`;
            } else {
                $scope.readerTop = `${iconEle.offsetTop - contentBody.scrollTop + 30}px`;
            }
            $scope.$apply();
        });
    });

    $scope.html = "";
    Object.defineProperty($scope, "shouldShowReader", {
        get: () => (!!$scope.html) && ($scope.isPageview) && (!$scope.isSearchMode),
    });

    $scope.$on("ExternalReaderContentReady", function(event, html) {
        $scope.html = $sce.trustAsHtml(html);
        iframe.contentWindow.focus();
    });
    $scope.$on("ExternalReaderClose", function(event, reason) {
        $log.log(`ExternalReaderClose, reason: ${reason}`);
        if (iconEle) {
            iconEle.classList.remove("active");
        }
        $scope.html = "";
        $scope.$apply();
    });

    $scope.externalRead = function(link, linkEle) {
        let {
            markdownDir,
            fromHeaderId,
            toHeaderId,
        } = ExternalReaderService.parseExternalLink(link);
        if (iconEle) {
            // if previous anchor icon is still active
            iconEle.classList.remove("active");
        }
        iconEle = linkEle.querySelector("span.icon");
        ExternalReaderService.loadExternalDManual(markdownDir, fromHeaderId, toHeaderId)
            .then(function(html) {
                $rootScope.$broadcast("ExternalReaderContentReady", html);
                iconEle.classList.add("active");
            }, function(error) {
                let TEXT_FAILED_TO_OPEN_EXTERNAL = gettext("TEXT_FAILED_TO_OPEN_EXTERNAL");
                $rootScope.$broadcast("ExternalReaderContentReady",
                    `<div style='color: red'>${TEXT_FAILED_TO_OPEN_EXTERNAL}</div>`);
                iconEle.classList.add("active");
            });
        let spaceholder = content.contentDocument.querySelector("footer.__spaceholder");
        let iconOffsetLeftToPage = iconEle.offsetLeft - spaceholder.offsetLeft;
        let pageWidth = parseInt(content.contentWindow.getComputedStyle(spaceholder).width);
        let klass;

        // make sure this is valid
        // determine reader on left/center/right
        if ((0 <= iconOffsetLeftToPage) && (iconOffsetLeftToPage < pageWidth / 3)) {
            // left
            klass = "onLeft";
        } else if ((pageWidth / 3 < iconOffsetLeftToPage) && (iconOffsetLeftToPage < pageWidth / 3 * 2)) {
            // center
            klass = "onCenter";
        } else {
            klass = "onRight";
        }

        // determine reader on top/bottom
        let iconOffsetTopToPage = iconEle.offsetTop - contentBody.scrollTop;
        let pageHeight = parseInt($window.getComputedStyle(content)["height"]);
        let onTop = iconOffsetTopToPage >= (pageHeight / 2);
        if (onTop) {
            klass += " onTop";
        } else {
            klass += " onBottom";
        }

        $scope.readerClasses = klass;
    };
    $window.externalRead = $scope.externalRead;

    angular.element($window).bind("contentScrollEvent", (event) => {
        $scope.$emit("ExternalReaderClose", "content-scrolled");
    });
});
