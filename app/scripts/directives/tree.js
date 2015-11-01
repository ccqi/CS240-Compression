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
      if (scope.tree.zero || scope.tree.one) {
        var ul = $compile('<ul></ul>')(scope);
        elm.append(ul);
        if (scope.tree.zero) {
          var zero = $compile('<tree ng-model="tree.zero"></tree>')(scope);
          elm.find('ul').append(zero);
        }   
        if (scope.tree.one) {
          var one = $compile('<tree ng-model="tree.one"></tree>')(scope);
          elm.find('ul').append(one);
        }
      }

      scope.$on('$destory', function(elm) {
        console.log('destroyed');
        elm.empty();
      });
         
    }
  };
  
});
