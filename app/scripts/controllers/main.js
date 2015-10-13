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

    $scope.submit = function() {
      var request = {
        'method': $scope.method,
        'data': $scope.data
      };
      C9nAPI.encode(request).then(
        function(response) {
          console.log('success');
          $scope.response = response.data;
          $scope.submitted = true;
        },
        function(error) {
          console.log('compression failed ');
        }
      );

    };
}]);

