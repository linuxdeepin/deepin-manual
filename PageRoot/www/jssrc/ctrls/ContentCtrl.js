"use strict";

angular.module("DManual")
    .controller("ContentCtrl", function($scope, $log, $window, $rootScope) {
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

        // listen to the event from MainCtrl, and fill the html into the iframe.
        $scope.$on("ContentHtmlReady", function(event, payload) {
            let { html, stylePath, scriptPath, markdownDir } = payload;
            let base = `<base href='${markdownDir}/'>
                <script src="${scriptPath}/mousetrap.js"></script>
                <script>
                'use strict';
                var disallow = function(event) {
                    event.preventDefault();
                    return false;
                };
                var emitEvent = function(eventName, eventMsg) {
                    var e = new CustomEvent(eventName, {detail: eventMsg});
                    window.parent.dispatchEvent(e);
                };
                window.onload = function() {
                    var body = document.body;
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
                ['scroll', 'click'].map(function(eventName){
                    document.addEventListener(eventName, function(){
                        emitEvent("searchBoxHideEvent");
                    });
                });
                document.addEventListener('scroll', function(e){
                    emitEvent("navigationRelocateEvent", {offset: window.scrollY});
                });
                </script>
                <link rel='stylesheet' href='${stylePath}/reset.css' />
                <link rel='stylesheet' href='${stylePath}/content.css' />`;
            let footer = `<footer class="__spaceholder"></footer>`;
            iframe.contentDocument.open();
            iframe.contentDocument.write(base + html + footer);
            iframe.contentDocument.close();
        });

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
