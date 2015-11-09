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
      scope.highlight = Highlight;
    }
  };
  
});
