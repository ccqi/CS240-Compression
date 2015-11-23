angular.module('Compression').factory('C9nAPI', ['$http', function($http) {
  return {
    encode : function(request) {
      return $http.post('/api/encode',request);
    },

    get: function(request) {
      return $http.post('api', request);
    },
    getTable: function(request) {
      return $http.post('api/table', request);
    }, 
    getData: function(request) {
      return $http.post('api/data', request);
    },

    save: function(request) {
      return $http.post('api/save', request);
    }
  } 
}]);
