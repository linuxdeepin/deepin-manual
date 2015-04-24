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

        // Auto-resize SideNavigationBar
        let _prevSideNavBarHeight = 0;
        let updateSidebar = function() {
            let newHeight = container.clientHeight - logoBox.clientHeight;
            if (newHeight !== _prevSideNavBarHeight) {
                $log.log(`SideNavigationBar height set to: ${newHeight}`);
                sideNavigationBar.style.height = newHeight + "px";
                _prevSideNavBarHeight = newHeight;
            }
            requestAnimationFrame(updateSidebar);
        };
        requestAnimationFrame(updateSidebar);

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
        $scope.isCollapsed = false;
        $scope.switchNavigationMode = function() {
            $scope.isCollapsed = !$scope.isCollapsed;
            $rootScope.$broadcast("navigationBarToggled", $scope.isCollapsed);
        };

        // Tooltip
        $scope.showTooltip = function(tooltip, $event) {
            let target = $event.target;

            AdapterService.showTooltip(
                tooltip,
                target.getBoundingClientRect());
        };
    });
