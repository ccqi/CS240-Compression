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
    $scope.types = [
      'binary',
      'hex'
    ];

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
          if ($scope.response.method == 'BWT') {
            $scope.types.push('text');
          }
          else{
            $scope.types = [
              'binary',
              'hex'
            ];
          }
          $scope.ratio = parseFloat($scope.response.encoding.compression_ratio).toFixed(2);
          if ($scope.response.encoding.huffmanTrie) {
            $scope.huffmanTree = $scope.response.encoding.huffmanTrie;
          }
          else {
            $scope.huffmanTree = undefined;
          }
          $scope.submitted = true;
        },
        function(error) {
          console.log('compression failed ');
        }
      );

    };
}]);

