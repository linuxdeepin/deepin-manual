"use strict";

angular.module("DManual")
    .controller("ContentCtrl", function($scope, $log, $window) {
        // Resize the block at the end of the content
        let iframe = document.getElementById("Content");
        $window.addEventListener("resize", function() {
            let offsets = $scope.anchorsOffsetList;
            let lastOffset = offsets[offsets.length - 1];
            let iframeDoc = iframe.contentDocument;
            let footer = iframeDoc.querySelector("footer");
            let h = iframe.getBoundingClientRect().height
                    - footer.offsetTop
                    + lastOffset
                    - parseInt($window.getComputedStyle(iframe)["padding-top"]);
            $log.log(`Set Content Footer to ${h}px`);
            footer.style.height = `${h}px`;
        });
    });
