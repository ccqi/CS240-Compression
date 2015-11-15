'use strict';

angular.module('Compression').controller('MainCtrl',
  ['$scope',
  'C9nAPI',
  '$compile',
  'Highlight',
  function($scope, C9nAPI, $compile, Highlight) {
    $scope.submitted = false;
    $scope.method = 'BWT';
    $scope.methods = [
      'BWT',
      'LZW',
      'MTF',
      'RLE',
      'Huffman'
    ]
    $scope.highlight = Highlight;
    $scope.dataChanged = function() {
      $scope.size = $scope.data.length * 4;
    }

    $scope.$watch('huffmanTree', function() {
      // destroy and recompile tree view whenever huffmanTree changes
      var tree = angular.element(document.querySelector('#root'));
      if(tree && tree.isolateScope()){
        tree.isolateScope().$destroy();
        var root = $compile('<tree id="root" ng-model="huffmanTree"></tree>')($scope);
        angular.element(document.querySelector('#rootContainer')).append(root);
      }
    });
    
    self.processOutput = function(output) {
      var textLen = 0;
      var info = {};
      for (var i = 0; i < output.length; i++) {
        var binary = '';
        var curBinary = output[i].binary;
        for(var j = 0; j < curBinary.length; j++) {
          textLen++;
          binary+= curBinary[j];
          if (textLen % 8 == 0) {
            binary += ' ';
          }
        }
        output[i].binary = binary;
      }
      return output;
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
            $scope.info = processOutput($scope.response.encoding.data);
          }

          $scope.ratio = parseFloat($scope.response.encoding.compression_ratio).toFixed(2);
          if ($scope.response.encoding.huffmanTrie) {
            $scope.huffmanTree = {
              'key': 'root',
              'value': $scope.response.encoding.huffmanTrie
            };
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

