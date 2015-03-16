"use strict";

var navigationCtrl = angular.module("DeepinHelp")
    .controller("NavigationBarCtrl", function($scope, $log) {
        var sideNavItems = document.getElementById("SideNavigationItems");
        angular.element(sideNavItems).on("wheel", function(event) {
            sideNavItems.scrollTop -= event.wheelDeltaY;
        });

        // auto-resize
        var container = document.getElementById("Container");
        var logoBox = document.getElementById("NavLogoBox");
        var updateSidebar = function() {
            console.log("resizing to " + (container.clientHeight - logoBox.clientHeight));
            $scope.sideNavBarStyle = {
                height: container.clientHeight - logoBox.clientHeight + "px"
            };
        };
        updateSidebar();
        angular.element(container).bind("resize", function(event) {
            updateSidebar();
            $scope.$apply();
        });
    });
