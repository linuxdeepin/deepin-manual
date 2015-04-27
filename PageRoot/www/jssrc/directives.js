"use strict";

let app = angular.module("DManual");

let templateUrlDirectiveConfigMap = {
    "deepinUiFrameBtn"          : "FrameBtn.html",
    "deepinUiManualContent"     : "ContentBox.html",
    "deepinUiManualNav"         : "NavigationBar.html",
    "deepinUiManualOverview"    : "Overview.html",
    "deepinUiManualSearchInput" : "SearchBox.html",
    "deepinUiManualSearchResult": "SearchResult.html"
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
app.directive("deepinAppManualBody", function(){
        return {
            restrict: "A",
            replace: true,
            link: function(scope, elem, attrs) {
                elem.addClass("overview-mode");
                scope.$watch("isCompactMode", function(value){
                    if(value) {
                        elem.addClass("compact-mode");
                    } else {
                        elem.removeClass("compact-mode");
                    }
                });
                scope.$watch("isSearchMode", function(value){
                    if(value) {
                        elem.addClass("search-mode");
                    } else {
                        elem.removeClass("search-mode");
                    }
                });
            }
        }
    })
    .directive("deepinUiSvgImage", function($http, $parse){
        return {
            restrict: "A",
            link: function(scope, elem, attrs) {
                let src = attrs.deepinUiSvgImage;
                // let src = attrs.ngSrc;
                if(!/\.svg$/.test(src)) {
                    let img = document.createElement('img');
                    img.src = src;
                    elem[0].appendChild(img);
                } else {
                    $http.get(src).then(function(res){
                        let div = document.createElement('div');
                        div.innerHTML = res.data;
                        let svg = div.querySelector('svg');
                        elem[0].appendChild(svg);
                    });
                }
            }
        }
    })
    .directive("deepinUiManualSearchFinish", function($timeout){
        return {
            restrict: "A",
            link: function (scope, element, attr) {
                if (scope.$last === true) {
                    $timeout(function () {
                        scope.$emit('manualSearchFinished');
                    });
                }
            }
        }
    });
