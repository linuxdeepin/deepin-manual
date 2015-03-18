"use strict";

var navigationCtrl = angular.module("DManual")
    .controller("NavigationBarCtrl", function($scope, $log) {
        var sideNavItems = document.getElementById("SideNavigationItems");
        angular.element(sideNavItems).on("wheel", function(event) {
            sideNavItems.scrollTop -= event.wheelDeltaY;
        });

        // auto-resize
        var container = document.getElementById("Container");
        var logoBox = document.getElementById("NavLogoBox");
        var updateSidebar = function() {
            let newHeight = container.clientHeight - logoBox.clientHeight;
            $log.log("resizing to " + newHeight + "px");
            $scope.sideNavBarStyle = {
                height: newHeight + "px",
            };
        };
        updateSidebar();
        angular.element(container).bind("resize", function(event) {
            updateSidebar();
            $scope.$apply();
        });
    });
