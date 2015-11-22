'use strict';

angular.module('Compression')
.constant('config', {
  'max': {
    'tree': '8',
    'data': '50',
    'table': '50',
    'tree': '3'
  },
  'popover': {
    'header': 'Algorithm',
    'sign': 'First bit of plain text',
    'paddingNum': 'Length of padding',
    'padding': 'Padding',
    'huffmanTree': 'Huffman tree'
  }
});
