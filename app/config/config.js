'use strict';

angular.module('Compression')
.constant('config', {
  'tooltip': {
    'header': 'This bit signifies the compression algorithm used',
    'sign': 'The first character of the original encoding',
    'paddingNum': 'The length of padding bits in binary',
    'padding': 'Padding',
    'huffmanTree': 'Huffman tree'
  }
});
