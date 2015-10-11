'use strict';

var myApp = angular.module('compression', ['ngRoute']);

myApp.controller('mainController', ['$scope', function($scope) {
  $scope.text = "hello world";
}]);

