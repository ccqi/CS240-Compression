angular.module('Compression').directive('data', function($compile, Highlight) {
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
      scope.popup = 'bye';
      scope.mouseOver = function(entry) {
        Highlight.mouseOver(scope.name, scope.type, entry);
        var elmClass = elm.children('.' + entry.field).children('uib-tooltip');
        elmClass.text('bye');
      };
      scope.mouseLeave = function(entry) {
        Highlight.mouseLeave(scope.name, scope.type, entry);
      };
    }
  };

});
