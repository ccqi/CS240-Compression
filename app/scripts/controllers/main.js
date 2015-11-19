'use strict';

angular.module('Compression').controller('MainCtrl',
  ['$scope',
  'C9nAPI',
  '$compile',
  'Highlight',
  'config',
  function($scope, C9nAPI, $compile, Highlight, config) {
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
   
    $scope.download = function() {
      C9nAPI.save().then(
        function(response) {
          var filename = response.data;
          var anchor = angular.element('<a/>');
          anchor.attr({
            href: 'files/' + filename,
            target: '_blank',
            download: filename
          })[0].click();
        }
      )  
    }

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
      output.textLength = textLen;
      return output;
    }
    self.processEntry = function(entry) {
      var len = $scope.info.textLength;
      var binary = '';
      var curBinary = entry.binary;
      for(var i = 0; i < curBinary.length; i++) {
        len++;
        binary += curBinary[i];
        if (len % 8 == 0) {
          binary += ' ';
        }
      }
      $scope.info.textLength = len;
      entry.binary = binary;
      return entry;
    }
    $scope.expand = function() {
      C9nAPI.getData({'type': $scope.method}).then(
        function(response){
          console.log('got more data...');
        
          var entries = response.data.data;
          $scope.response.encoding.percent = response.data.percent;
          var info = $scope.info
          for (var i = 0; i < entries.length; i++) {
            $scope.info.push(processEntry(entries[i]));
          }
        },
        function(response) {
          console.log('failed');
        }
      );

    };
    $scope.blur = function() {
      $('.btn').blur();
    }
    $scope.submit = function() {
      var request = {
        'method': $scope.method,
        'data': $scope.data
      };
      C9nAPI.encode(request).then(
        function(response) {
          console.log(response.data);
          var params = {
            'type': $scope.method,
            'max': config.max.table
          };
          C9nAPI.get(params).then(
            function(response) {
              console.log('got data');
              $scope.response = response.data;
              if ($scope.response.encoding.data) {
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
            }
          )
        },
        function(error) {
          console.log('compression failed ');
        }
      );

    };
}]);

