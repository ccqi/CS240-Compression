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
      self.textLen = 0;
      scope.getPopoverText = function(field) {
        return config.popover[field];
      };
      
      scope.mouseOver = function(entry) {
        Highlight.mouseOver(scope.name, scope.type, entry);
      };

      scope.mouseLeave = function(entry) {
        Highlight.mouseLeave(scope.name, scope.type, entry);
      };
    }
  };

});
