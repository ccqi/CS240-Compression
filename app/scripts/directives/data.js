angular.module('Compression').directive('data', function($compile, Highlight, config) {
  return {
    templateUrl: 'views/data.html',
    replace: true,
    transclude: true,
    restrict: 'E',
    scope: {
      data: '=ngModel',
      type: '='
    },
    link: function(scope, elm, attrs) {
      var self = this;
      scope.name = 'data';
      scope.highlight = Highlight;
      scope.mouseOver = function(entry) {
        Highlight.mouseOver(scope.name, scope.type, entry);
        scope.tooltipText = config.tooltip[entry.field];
      };
      scope.mouseLeave = function(entry) {
        Highlight.mouseLeave(scope.name, scope.type, entry);
        //scope.tooltipText = config.tooltip[entry.field];
      };
    }
  };

});
