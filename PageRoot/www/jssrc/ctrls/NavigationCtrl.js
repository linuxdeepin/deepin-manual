"use strict";

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

        let navigationRelocate = function(offset) {
            let offsetList = $scope.anchorsOffsetList;
            let lastIndex = offsetList.length - 1;
            $scope.navigations.map(function(headerNode){
                headerNode.classList.remove('current-section');
            });
            for (var i = 0; i < offsetList.length; i++) {
                if(offset <= offsetList[i]) {
                    if($scope.navigations[i].classList.contains("level3")){
                        $scope.navigations[i].parentNode.parentNode.classList.add('current-section');
                    }
                    $scope.navigations[i].classList.add('current-section');
                    return;
                } else if(offset > offsetList[i] && offset <= offsetList[i+1]) {
                    if($scope.navigations[i+1].classList.contains("level3")){
                        $scope.navigations[i+1].parentNode.parentNode.classList.add('current-section');
                    }
                    $scope.navigations[i+1].classList.add('current-section');
                    return;
                }
            }
        };

        $scope.$on("navigationRelocate", function(event, value){
            let offset = value;
            navigationRelocate(offset);
        });
        $window.addEventListener("navigationRelocateEvent", function(e) {
            let offset = e.detail.offset;
            navigationRelocate(offset);
        });
        $scope.isCollapsed = false;
        $scope.switchNavigationMode = function() {
            $scope.isCollapsed = !$scope.isCollapsed;
            $rootScope.$broadcast("navigationBarToggled", $scope.isCollapsed);
        };
        $scope.showTooltip = function(tooltip, $event) {
            let target = $event.target;

            AdapterService.showTooltip(
                tooltip,
                target.getBoundingClientRect());
        }
    });
