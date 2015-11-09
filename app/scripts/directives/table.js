angular.module('Compression').directive('encodingTable', function($compile, Highlight) {
  var self = this;
  
  return {
    templateUrl: 'views/table.html',
    replace: true,
    transclude: true,
    restrict: 'E',
    scope: {
      type: '=',
      table:'=ngModel'
    },
    link: function(scope, elm, attrs) {
      scope.highlight = Highlight;
    }
  };
  
});
