angular.module('Compression').directive('encodingTable', function($compile, $window, C9nAPI, Highlight, config) {
  var self = this;

  return {
    templateUrl: 'views/table.html',
    replace: true,
    transclude: true,
    restrict: 'E',
    scope: {
      type: '=',
      table:'=ngModel',
      filename: '='
    },
    link: function(scope, elm, attrs) {
      scope.name = 'table';
      scope.highlight = Highlight;
      scope.MORE = 'See more...'
      scope.getScrollBarWidth = function() {
        // Create the measurement node
        var scrollDiv = document.createElement("div");
        scrollDiv.className = "scrollbar-measure";
        document.body.appendChild(scrollDiv);

        // Get the scrollbar width
        scope.scrollbarWidth = scrollDiv.offsetWidth - scrollDiv.clientWidth;

        // Delete the DIV
        document.body.removeChild(scrollDiv);
        scope.start = parseInt(config.max.table);
      };

      scope.expand = function() {
        if (scope.start >= config.absoluteMax.table) {
          return;
        }
        C9nAPI.getTable({
          'filename': scope.filename,
          'type': scope.type,
          'increment': config.max.table,
          'start': scope.start
        }).then(
          function(response){
            console.log('got more table data...');
            var entries = response.data.entries;
            scope.table.percent = response.data.percent;
            for (var i = 0; i < entries.length; i++) {
              scope.table.entries.push(entries[i]);
            }
            scope.start += parseInt(config.max.table);
            if (scope.start >= config.absoluteMax.table) {
              scope.MORE = 'This table is too large to show more entries'
            }
          },
          function(response) {
            console.log('failed');
          }
        )
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
