'use strict';

angular.module('Compression').controller('MainCtrl', 
  ['$scope', 
  'C9nAPI',
  function($scope, C9nAPI) {
    $scope.submitted = false;
    $scope.method = 'BWT';
    $scope.methods = [
      'BWT',
      'LZW',
      'MTF',
      'RLE',
      'Huffman'
    ]
    $scope.dataChanged = function() {
      $scope.size = $scope.data.length * 4;
    }
    
    $scope.submit = function() {
      var request = {
        'method': $scope.method,
        'data': $scope.data
      };
      C9nAPI.encode(request).then(
        function(response) {
          console.log('success');
          $scope.response = response.data;
          if ($scope.response.encoding.data){
            $scope.info = $scope.response.encoding.data;
          }
          $scope.submitted = true;
        },
        function(error) {
          console.log('compression failed ');
        }
      );

    };
}]);

