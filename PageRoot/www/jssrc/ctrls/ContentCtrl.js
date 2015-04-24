"use strict";

angular.module("DManual")
    .controller("ContentCtrl", function($scope, $log, $window) {
        // Resize the block at the end of the content
        let iframe = document.getElementById("Content");
        let _prevIframeHeight = -1;
        let resizeSpaceHolder = function() {
            let iframeHeight = iframe.getBoundingClientRect().height;
            if (iframeHeight === 0) {
                return requestAnimationFrame(resizeSpaceHolder);
            }
            if (iframeHeight === _prevIframeHeight) {
                return requestAnimationFrame(resizeSpaceHolder);
            }
            _prevIframeHeight = iframeHeight;
            let offsets = $scope.anchorsOffsetList;
            let lastOffset = offsets[offsets.length - 1];
            let iframeDoc = iframe.contentDocument;
            let footer = iframeDoc.querySelector("footer");
            let h = iframeHeight
                - footer.offsetTop
                + lastOffset
                - parseInt($window.getComputedStyle(iframe)["padding-top"]);
            if (h >= 0) {
                $log.log(`Set Content Footer to ${h}px`);
                footer.style.height = `${h}px`;
            } else {
                $log.log(`Set Content Footer to 0px`);
                footer.style.height = `0px`;
            }
            requestAnimationFrame(resizeSpaceHolder);
        };
        requestAnimationFrame(resizeSpaceHolder);
    });
