let app = angular.module("DManual");

let templateUrlDirectiveConfigMap = {
  "deepinUiFrameBtn"          : "FrameBtn.html",
  "deepinUiManualContent"     : "ContentBox.html",
  "deepinUiManualNav"         : "NavigationBar.html",
  "deepinUiManualOverview"    : "Overview.html",
  "deepinUiManualSearchInput" : "SearchBox.html",
  "deepinUiManualSearchResult": "SearchResult.html"
}

// Custom UI controls here
app.directive("deepinUiFrame", function(){
    return {
      restrict: "E",
      replace: "true",
      template: `<div id="OuterFrame" ng-style="outerFrameStyle">
        <div id="MoveHandle"></div>
      </div>`
    }
  })

;(function(app, directives){
  Object.keys(directives).forEach(function(directive){
    app.directive(directive, function(){
      return {
        restrict: "E",
        replace: "true",
        templateUrl: "./partials/" + directives[directive]
      }
    });
  });
})(app, templateUrlDirectiveConfigMap)

// Custom Attributes here
app.directive("deepinAppManualBody", function($rootScope){
    return {
      restrict: "A",
      replace: true,
      link: function(scope, elem, attrs) {
        elem.addClass("overview-mode");
        scope.$watch("isCompactMode", function(isCompactMode){
          if(isCompactMode) {
            elem.addClass("compact-mode");
          } else {
            elem.removeClass("compact-mode");
          }
        });
      }
    }
  })