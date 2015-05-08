"use strict";

let app = angular.module("DManual");

let templateUrlDirectiveConfigMap = {
    "deepinUiFrameBtn"          : "FrameBtn.html",
    "deepinUiManualContent"     : "ContentBox.html",
    "deepinUiManualNav"         : "NavigationBar.html",
    "deepinUiManualOverview"    : "Overview.html",
    "deepinUiManualSearchInput" : "SearchBox.html"
};

// Custom UI controls here
app.directive("deepinUiFrame", function(){
    return {
        restrict: "E",
        replace: "true",
        template: `<div id="MoveHandle" class="move-handle" ng-controller="FrameCtrl"><!-- div id="OuterFrame" ng-style="outerFrameStyle"></div--></div>`,
    }
  }).directive("deepinUiContainer", function(){
    return {
        restrict: "E",
        replace: "true",
        transclude: "true",
        template: `<div id="Container">
            <ng-transclude></ng-transclude>
        </div>`
    }
  })

;(function(app, directives){
    Object.keys(directives).map(function(directive){
        app.directive(directive, function(){
            return {
                restrict: "E",
                replace: "true",
                templateUrl: "./partials/" + directives[directive]
            }
        });
    });
})(app, templateUrlDirectiveConfigMap);

// Custom Attributes here
app.directive("deepinUiSvgImage", function($http){
    return {
        restrict: "A",
        link: function(scope, elem, attrs) {
            let src = attrs.deepinUiSvgImage;
            // let src = attrs.ngSrc;
            if(!/\.svg$/.test(src)) {
                let img = angular.element(`<img src="${src}"/>`);
                elem.append(img);
            } else {
                $http.get(src).then(function(res){
                    let svg = angular.element(`<div>${res.data}</div>`).find('svg');
                    elem.append(svg);
                });
            }
        }
    }
});