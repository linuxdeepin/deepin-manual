"use strict";

angular.module("DManual").controller("FrameCtrl", function($scope, AdapterService) {
    if (AdapterService.getShellType() === "DAE") {
        let moveHandle = document.getElementById("MoveHandle");
        moveHandle.addEventListener("mousedown", AdapterService.moveHandleDown);
        moveHandle.addEventListener("mouseup", AdapterService.moveHandleUp);
        moveHandle.addEventListener("dblclick", AdapterService.moveHandleDblclick);
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
