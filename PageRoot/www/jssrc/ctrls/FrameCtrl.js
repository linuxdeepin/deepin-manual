"use strict";

angular.module("DManual").controller("FrameCtrl", function($scope, AdapterService) {
    if (AdapterService.getShellType() === "DAE") {
        ["MoveHandle","MoveHandleExtra"].map(function(elId){
            let el = document.getElementById(elId);
            el.addEventListener("mousedown", AdapterService.moveHandleDown);
            el.addEventListener("mouseup", AdapterService.moveHandleUp);
            el.addEventListener("dblclick", AdapterService.moveHandleDblclick);
        });
    }

    //let updateOuterFrame = function() {
    //    $scope.outerFrameStyle = {
    //        height: $window.innerHeight + "px",
    //    };
    //};
    //updateOuterFrame();
    //angular.element($window).bind("resize", function(event) {
    //    updateOuterFrame();
    //});
});
