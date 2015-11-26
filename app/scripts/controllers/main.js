'use strict';

angular.module('Compression').controller('MainCtrl',
  ['$scope',
  '$q',
  'C9nAPI',
  '$compile',
  'Highlight',
  'config',
  'Upload',
  function($scope, $q, C9nAPI, $compile, Highlight, config, Upload) {
    $scope.submitted = false;
    $scope.fileChanged = false;
    $scope.method = 'BWT';
    $scope.methods = [
      'BWT',
      'LZW',
      'MTF',
      'RLE',
      'Huffman'
    ]
    $scope.col = {
      'BWT': 'col-md-12',
      'LZW': 'col-md-6',
      'MTF': 'col-md-12',
      'RLE': 'col-md-6',
      'Huffman': 'col-md-5'
    };
    $scope.config = config;
    $scope.highlight = Highlight;
    $scope.notAscii = false;
    $scope.dataChanged = function() {
      if ($scope.data) {
        $scope.size = $scope.data.length * 8;
      }
      else {
        $scope.size = 0;
      }
    }
    $scope.MORE = {
      'DATA': 'See more...',
      'TREE': 'This is a partial tree. The full tree is too large to be entirely displayed on this page.'
    };

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
    
    $scope.fileError = function() {
      console.log("File",form.file.$error);
      if ($scope.file && form.file.$error) {
        console.log("Error", form.file.$error);
        return true; 
      }
      return false;
    };


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
    };

    $scope.newUpload = function($file) {
      $scope.file = $file;
      if ($scope.file) {
        $scope.size = $scope.file.size * 8;
        $scope.fileChanged = true;
      }
    };

    $scope.initFiles = function($file) {
      if ($scope.file) {
        $scope.size = $scope.file.size * 8;
      }
    }

    $scope.expand = function() {
      if ($scope.start >= config.absoluteMax.data) {
        return;
      }
      C9nAPI.getData({
        'filename': $scope.filename,
        'type': $scope.method,
        'start': $scope.start,
        'increment': config.max.data
      }).then(
        function(response){
          console.log('got more data...');
          $scope.start += parseInt(config.max.data); 
          var entries = response.data.data;
          $scope.response.encoding.percent = response.data.percent;
          var info = $scope.info
          for (var i = 0; i < entries.length; i++) {
            $scope.info.push(processEntry(entries[i]));
          }
          if ($scope.start >= config.absoluteMax.data) {
            $scope.MORE.DATA = 'To see more, download the entire encoding...';
          }
        },
        function(response) {
          console.log('failed');
        }
      );

    };

    $scope.blur = function() {
      $('.btn').blur();
    };

    $scope.upload = function (file) {
      var deferred = $q.defer();
      Upload.upload({
          url: 'api/upload',
          file: file
      }).then(function (resp) {
          deferred.resolve(resp.data.filename);
      }, function (resp) {
          deferred.reject('error');
      });
      return deferred.promise;
    };
    
    $scope.encode = function(request) {
      C9nAPI.encode(request).then(
        function(response) {
          console.log('compression success');
          $scope.notAscii = false;
          $scope.response = response.data;         
          $scope.filename = $scope.response.filename;
          if ($scope.response.encoding.data) {
            $scope.info = processOutput($scope.response.encoding.data);
          }
          $scope.ratio = parseFloat($scope.response.encoding.compression_ratio).toFixed(2);
          $scope.start = parseInt(config.max.data);
          if ($scope.start >= config.absoluteMax.data) {
            $scope.MORE.DATA = 'To see more, download the entire encoding...';
          }
          else {
            $scope.MORE.DATA = 'See more...'
          }
          if ($scope.response.encoding.huffmanTrie) {
            $scope.huffmanTree = {
              'key': 'root',
              'percent': $scope.response.encoding.treePercentage,
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
          if (error.status == 4001) {
            $scope.notAscii = true;
          } else {
            $scope.notAscii = false;
          }
        }
      );
    };

    $scope.submit = function(file) {
      var request = {
        'method': $scope.method,
        'max': config.max
      };

      if ($scope.showFile) {
        if ($scope.file) {
          if ($scope.fileChanged) {
            $scope.upload($scope.file).then(
              function(res) {
                console.log('successfully saved file');
                request.inputType = 'FILE';
                $scope.fileRef = res;
                request.content = res;
                $scope.encode(request);
                $scope.fileChanged = false;
              },
              function(err) {
                console.error(err);
              }
            );
          } else {
            request.inputType = 'FILE';
            request.content = $scope.fileRef;
            $scope.encode(request);
          }
        }
        else {
          console.error('You need to upload a file');
          return;
        }
      }
      else {
        request.inputType = 'TEXT';
        request.content = $scope.data;
        $scope.encode(request);
      }
    };
}]);

