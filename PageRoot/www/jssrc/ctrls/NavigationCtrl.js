"use strict";

angular.module("DManual")
    .controller("NavigationBarCtrl", function($scope, $rootScope, $log, $window) {
        let sideNavItems = document.getElementById("SideNavigationItems");
        angular.element(sideNavItems).on("wheel", function(event) {
            sideNavItems.scrollTop -= event.wheelDeltaY;
        });
        // auto-resize
        let container = document.getElementById("Container");
        let logoBox = document.getElementById("NavLogoBox");
        let updateSidebar = function() {
            let newHeight = container.clientHeight - logoBox.clientHeight;
            $log.log("resizing to " + newHeight + "px");
            $scope.sideNavBarStyle = {
                height: newHeight + "px",
            };
        };
        updateSidebar();
        angular.element($window).bind("resizeSidebar", function(event) {
            updateSidebar();
        });
        angular.element($window).bind("resize", function(event) {
            updateSidebar();
            $scope.$apply();
        });
        $scope.isCollapsed = false;
        $scope.switchNavigationMode = function() {
            $scope.isCollapsed = !$scope.isCollapsed;
            $rootScope.$broadcast("navigationBarToggled", $scope.isCollapsed);
        }
    });
