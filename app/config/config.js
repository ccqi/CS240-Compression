'use strict';

angular.module('Compression')
.constant('config', {
  'max': {
    'data': '50',
    'table': '50',
    'tree': '3'
  },
  'absoluteMax': {
    'data': '300',
    'table': '300'
  },
  'popover': {
    'header': 'Algorithm',
    'sign': 'First bit of plain text',
    'paddingNum': 'Length of padding',
    'padding': 'Padding',
    'huffmanTree': 'Huffman tree'
  }
});
