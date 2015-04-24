"use strict";

let highlightNode = function(node) {
    if (node.classList.contains("level3")) {
        node.parentNode.parentNode.classList.add('current-section');
    }
    node.classList.add('current-section');
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
        sideBarStyle.barHeight = null;
        sideBarStyle.itemsHeight = null;
        sideBarStyle.upArrow = Object.create(null);
        sideBarStyle.downArrow = Object.create(null);

        // Auto-resize SideNavigationBar
        let _prevSideNavBarHeight = 0;
        let updateSidebar = function() {
            // resize SideNavigationBar
            let newBarHeight = container.clientHeight - logoBox.clientHeight;
            if (newBarHeight !== _prevSideNavBarHeight) {
                sideBarStyle.barHeight = newBarHeight;
                sideBarStyle.itemsHeight = newBarHeight - upArrow.clientHeight - downArrow.clientHeight;
                _prevSideNavBarHeight = newBarHeight;
            }

            // up/down arrows
            let atTheTopOfScroll = false;
            let atTheEndOfScroll = false;

            if (sideNavItems.scrollTop === 0) {
                atTheTopOfScroll = true;
            }
            if (sideNavItems.scrollHeight - sideNavItems.scrollTop === sideNavItems.clientHeight) {
                // at the end of the scroll
                atTheEndOfScroll = true;
            }

            if (atTheTopOfScroll && atTheEndOfScroll) {
                // no need to show arrows at all
                sideBarStyle.upArrow.invisibility = "hidden";
                sideBarStyle.downArrow.invisibility = "hidden";
            } else {
                sideBarStyle.upArrow.invisibility = "visible";
                sideBarStyle.downArrow.invisibility = "visible";

                if (atTheTopOfScroll) {
                    sideBarStyle.upArrow.background = "red";
                } else {
                    sideBarStyle.upArrow.background = "pink";
                }
                if (atTheEndOfScroll) {
                    sideBarStyle.downArrow.background = "red";
                } else {
                    sideBarStyle.downArrow.background = "pink";
                }
            }
            sideNavigationBar.style.height = sideBarStyle.barHeight + "px";
            sideNavItems.style.height = sideBarStyle.itemsHeight + "px";
            upArrow.style.visibility = sideBarStyle.upArrow.invisibility;
            upArrow.style.background = sideBarStyle.upArrow.background;
            downArrow.style.visibility = sideBarStyle.downArrow.invisibility;
            downArrow.style.background = sideBarStyle.downArrow.background;
            requestAnimationFrame(updateSidebar);
        };
        requestAnimationFrame(updateSidebar);

        // mouse event
        angular.element(sideNavItems).on("wheel", function(event) {
            sideNavItems.scrollTop -= event.wheelDeltaY;
        });

        // Content Scroll
        let navigationRelocate = function(offset) {
            let offsetList = $scope.anchorsOffsetList;
            let lastIndex = offsetList.length - 1;
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

        $scope.$on("navigationRelocate", function(event, value) {
            // triggers when jumpTo
            let offset = value;
            navigationRelocate(offset);
        });
        $window.addEventListener("navigationRelocateEvent", function(e) {
            // triggers when mousewheel on IFrame
            let offset = e.detail.offset;
            navigationRelocate(offset);
        });

        // Sidebar Collapse/Expand
        $scope.isCollapsed = AdapterService.isCompactMode();
        $scope.toggleNavigationCompactMode = function() {
            $scope.isCollapsed = !$scope.isCollapsed;
            $rootScope.$broadcast("navigationBarToggled", $scope.isCollapsed);
        };
        $rootScope.$broadcast("navigationBarToggled", $scope.isCollapsed);

        // Tooltip
        $scope.showTooltip = function(tooltip, $event) {
            let target = $event.target;

            AdapterService.showTooltip(
                tooltip,
                target.getBoundingClientRect());
        };
    });
