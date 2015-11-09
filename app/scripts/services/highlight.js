angular.module('Compression').factory('Highlight', ['$http', function($http) {
  var self = this;
  self.changeColor = function(elms, color) {
    for (var i = 0; i <  elms.length; i++ ) {
      elms[i].style.backgroundColor = color;
    }
  }

  self.mouseOver = function(type, entry) {
    var elms = document.getElementsByClassName(entry.field);
    console.log(type);
    self.changeColor(elms, "yellow");
    
    if (type === 'Huffman' && entry.field !== 'huffmanTree' && entry.field.indexOf('huffman') > -1 ) {
      root = $('#root');
      self.traverseTree(root, 0, entry.binary, true); 
    }
  };

  self.mouseLeave = function(type, entry) {
    var elms = document.getElementsByClassName(entry.field);
    self.changeColor(elms, "inherit");
    if (type === 'Huffman' && entry.field !== 'huffmanTree' && entry.field.indexOf('huffman') > -1 ) {
      root = $('#root');
      self.traverseTree(root, 0, entry.binary, false); 
    }
  };
  
  self.traverseTree = function(element, index, code, flag) {
    if (index <= code.length) {
      if (index > 0) {
        if (flag) {
          element.children('.leaf').css('background-color','#c8e4f8');
        } else {
          element.children('.leaf').removeAttr('style');
        }
      }

      var ul = element.children('ul');
      if (code[index] === '0') {
        self.traverseTree(ul.children('.zero'),index + 1, code, flag);
      }
      else {
        self.traverseTree(ul.children('.one'),index + 1, code, flag);
      }
    }
  }
  return {
    mouseOver : self.mouseOver,
    mouseLeave: self.mouseLeave
  } 
}]);
