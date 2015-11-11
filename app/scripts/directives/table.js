angular.module('Compression').directive('encodingTable', function($compile, $window, Highlight) {
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
      scope.name = 'table';
      scope.highlight = Highlight;
      scope.getScrollBarWidth = function() {
        // Create the measurement node
        var scrollDiv = document.createElement("div");
        scrollDiv.className = "scrollbar-measure";
        document.body.appendChild(scrollDiv);

        // Get the scrollbar width
        scope.scrollbarWidth = scrollDiv.offsetWidth - scrollDiv.clientWidth;

        // Delete the DIV
        document.body.removeChild(scrollDiv);
      };
      scope.resizeTable = function() {
        $('.table-head').removeAttr('style');
        width = $('.table-head').width() - scope.scrollbarWidth;
        $('.table-head').css('width', width);
      };
      scope.getScrollBarWidth();
      scope.resizeTable();
      angular.element($window).bind('resize', function() {
        scope.resizeTable();
        scope.$apply();
      });
    }
  };
});
