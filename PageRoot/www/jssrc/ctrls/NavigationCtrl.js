"use strict";

let highlightNode = function(node) {
    if (node.classList.contains("level3")) {
        node.parentNode.parentNode.classList.add('current-section');
    }
    node.classList.add('current-section');
    node.scrollIntoView();
};

angular.module("DManual")
    .controller("NavigationBarCtrl", function($scope, $rootScope, $log, $window, AdapterService) {
        let container = document.getElementById("Container");
        let logoBox = document.getElementById("NavLogoBox");
        let sideNavigationBar = document.getElementById("SideNavigationBar");
        let sideNavItems = document.querySelector("ol#SideNavigationItems");
        let upArrow = document.getElementById("SideNavUpArrow");
        let downArrow = document.getElementById("SideNavDownArrow");

        // declare structure of sideBarStyle
        let sideBarStyle = Object.create(null);
        sideBarStyle.bar = Object.create(null);
        sideBarStyle.items = Object.create(null);
        sideBarStyle.upArrow = Object.create(null);
        sideBarStyle.downArrow = Object.create(null);

        // Auto-resize SideNavigationBar
        let updateSidebar = function() {
            // resize SideNavigationBar
            let newBarHeight = container.clientHeight - logoBox.clientHeight;

            // TODO: make isCompactMode a property, that takes isSearchMode into consideration.
            if ($scope.isCompactMode || $scope.isSearchMode) {
                let itemsScrollHeight = sideNavItems.scrollHeight;

                // show/hide arrows
                if (itemsScrollHeight <= newBarHeight) {
                    // hide arrows
                    sideBarStyle.upArrow.display = "none";
                    sideBarStyle.downArrow.display = "none";
                    sideBarStyle.items.height = newBarHeight;
                } else {
                    // show arrows
                    sideBarStyle.upArrow.display = "block";
                    sideBarStyle.downArrow.display = "block";
                    sideBarStyle.items.height = newBarHeight - upArrow.clientHeight - downArrow.clientHeight;

                    // arrow style
                    let atTheTopOfScroll = false;
                    let atTheEndOfScroll = false;
                    if (sideNavItems.scrollTop === 0) {
                        atTheTopOfScroll = true;
                    }
                    if (sideNavItems.scrollHeight - sideNavItems.scrollTop === sideNavItems.clientHeight) {
                        // at the end of the scroll
                        atTheEndOfScroll = true;
                    }

                    if (atTheTopOfScroll) {
                        upArrow.classList.add("normal");
                        upArrow.classList.remove("end");
                    } else {
                        upArrow.classList.add("end");
                        upArrow.classList.remove("normal");
                    }
                    if (atTheEndOfScroll) {
                        downArrow.classList.add("normal");
                        downArrow.classList.remove("end");
                    } else {
                        downArrow.classList.add("end");
                        downArrow.classList.remove("normal");
                    }
                }

                // hide scrollbar
                sideBarStyle.items.overflowY = "hidden";
            } else {
                // hide arrows
                sideBarStyle.upArrow.display = "none";
                sideBarStyle.downArrow.display = "none";

                // set items height
                sideBarStyle.items.height = newBarHeight;

                // scrollbar -> auto
                sideBarStyle.items.overflowY = "auto";
            }

            // apply styles
            sideNavigationBar.style.height = sideBarStyle.bar.height + "px";
            sideNavItems.style.height = sideBarStyle.items.height + "px";
            sideNavItems.style.overflowY = sideBarStyle.items.overflowY;
            upArrow.style.display = sideBarStyle.upArrow.display;
            downArrow.style.display = sideBarStyle.downArrow.display;
            requestAnimationFrame(updateSidebar);
        };
        requestAnimationFrame(updateSidebar);

        // mouse event
        angular.element(sideNavItems).on("mousewheel", function(event) {
            sideNavItems.scrollTop -= event.wheelDeltaY;
        });

        // Content Scroll
        let navigationRelocate = function(offset) {
            let offsetList = $scope.anchorsOffsetList;
            let lastIndex = offsetList.length - 1;

            if (!offsetList || lastIndex === -1) {
                $log.warn("navigationRelocate abort due to lack of anchors");
                return;
            }

            $scope.navigations.map(function(headerNode) {
                headerNode.classList.remove('current-section');
            });
            for (let i = 0; i < offsetList.length; i++) {
                if (i !== lastIndex) {
                    if (offsetList[i+1] > offset) {
                        return highlightNode($scope.navigations[i]);
                    }
                }
            }
            return highlightNode($scope.navigations[lastIndex]);
        };

        $window.addEventListener("navigationRelocateEvent", function(e) {
            // triggers when scroll on IFrame
            let offset = e.detail.offset;
            navigationRelocate(offset);
        });

        // Sidebar CompactMode
        $scope.toggleNavigationCompactMode = function() {
            let newValue = !$scope.isCompactMode;
            // MainCtrl needs to know about this
            $scope.$emit("SideNavCompactModeSet", newValue);
            AdapterService.setCompactMode(newValue);
        };
        // MainCtrl needs to know about this
        $scope.$emit("SideNavCompactModeSet", AdapterService.isCompactMode());

        // Tooltip
        $scope.showTooltip = function(tooltip, $event) {
            if ($scope.isCompactMode) {
                let target = $event.target;

                AdapterService.showTooltip(
                    tooltip,
                    target.getBoundingClientRect());
            }
        };
    });
