'use strict';

angular.module('Compression').controller('MainCtrl', 
  ['$scope', 
  'C9nAPI',
  '$compile',
  function($scope, C9nAPI, $compile) {
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

    $scope.$watch('huffmanTree', function() {
      // destroy and recompile tree view whenever huffmanTree changes
      var tree = angular.element(document.querySelector('#root'));
      if(tree && tree.isolateScope()){
        tree.isolateScope().$destroy();
        var root = $compile('<tree id="root" ng-model="huffmanTree"></tree>')($scope);
        angular.element(document.querySelector('#rootContainer')).append(root);
      }
    });
   
    var changeColor = function(elms, color) {
      for (var i = 0; i <  elms.length; i++ ) {
        elms[i].style.backgroundColor = color;
      }
    }

    $scope.mouseOver = function(className) {
      var elms = document.getElementsByClassName(className);
      changeColor(elms, "yellow");
    };

    $scope.mouseLeave = function(className) {
      var elms = document.getElementsByClassName(className);
      changeColor(elms, "inherit");
    };
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

