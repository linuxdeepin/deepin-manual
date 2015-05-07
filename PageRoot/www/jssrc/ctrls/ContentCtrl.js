"use strict";

angular.module("DManual")
    .controller("ContentCtrl", function($scope, $log, $window, $rootScope,
                                        MarkdownService) {
        let iframe = document.getElementById("Content");
        iframe.addEventListener("load", function() {
            $scope.$emit("ContentHtmlLoaded");
        });

        // Resize the block at the end of the content
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

        let onMarkdownProcessed = function() {
            iframe.contentDocument.open();
            iframe.contentDocument.write(MarkdownService.getHtml());
            iframe.contentDocument.close();
        };
        $scope.$on("MarkdownProcessed", onMarkdownProcessed);
        if (MarkdownService.isInitialized()) {
            onMarkdownProcessed();
        }

        // JumpTo
        let _realJumpTo = function(anchor) {
            let contentWin = iframe.contentWindow;
            if (anchor) {
                // set hash to empty first,
                // Browser will not do anything if a same hash is set again
                contentWin.location.hash = "";
                contentWin.location.hash = encodeURIComponent(anchor);
                $rootScope.$broadcast("navigationRelocate", contentWin.scrollY);
            } else {
                contentWin.location.hash = "";
                $rootScope.$broadcast("navigationRelocate", contentWin.scrollY);
            }
        };
        $scope.$on("jumpTo", function(event, anchor) {
            _realJumpTo(anchor);
        });
    });
