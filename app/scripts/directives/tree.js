angular.module('Compression').directive('tree', function($compile, Highlight) {
  return {
    templateUrl: 'views/tree.html',
    replace: true,
    transclude: true,
    restrict: 'E',
    scope: {
      tree: '=ngModel'
    },
    link: function(scope, elm, attrs) {
      var self = this;
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
      self.addStyle = function(elm, color) {
        if (color) { 
          elm.children('.leaf').css('background-color', color);
        } else {
          elm.children('.leaf').removeAttr('style');
        }
      };
      self.traverseTop = function(elm, color) {
        if (elm.is('#root')) {
          self.addStyle(elm, color);
          return '';
        }
        if (elm.hasClass('zero')) {
          self.addStyle(elm, color);
          return traverseTop(elm.parent().parent(), color) + '0';
        }
        else if (elm.hasClass('one')) {
          self.addStyle(elm, color);
          return traverseTop(elm.parent().parent(), color) + '1';
        }
        else {
          throw 'something went wrong';
        }
      }
      scope.mouseOver = function() {
        var code = {'field': traverseTop(elm, 'yellow')};
        Highlight.mouseOver('data', code);
      };
      scope.mouseLeave = function() {
        var code = {'field': traverseTop(elm)};
        Highlight.mouseLeave('data', code);
      };
    }
  };
  
});
