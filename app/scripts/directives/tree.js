angular.module('Compression').directive('tree', function($compile) {
  return {
    templateUrl: 'views/tree.html',
    replace: true,
    transclude: true,
    restrict: 'E',
    scope: {
      tree: '=ngModel'
    },
    link: function(scope, elm, attrs) {
      scope.$watch('tree', function() {
      if (scope.tree.value.zero || scope.tree.value.one) {
        var ul = $compile('<ul></ul>')(scope);
        elm.append(ul);
        
        if (scope.tree.value.zero) {
          scope.zeroTree = {
            'key': {
              'zero': '0',
              'one': ''
            
            },
            'value': scope.tree.value.zero
          };
          var zero = $compile('<tree ng-model="zeroTree"></tree>')(scope);
          elm.find('ul').append(zero);
        }   
        if (scope.tree.value.one) {
          scope.oneTree = {
            'key': {
              'one': '1',
              'zero': ''
            },
            'value': scope.tree.value.one
          };
          var one = $compile('<tree ng-model="oneTree"></tree>')(scope);
          elm.find('ul').append(one);
        }
      }
      scope.$on('$destroy', function() {
        elm.remove();
      });
      });
    }
  };
  
});
