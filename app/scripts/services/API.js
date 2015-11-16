angular.module('Compression').factory('C9nAPI', ['$http', function($http) {
  return {
    encode : function(request) {
      return $http.post('/api/encode',request);
    },

    get: function(request) {
      return $http.get('api', {
        params: request
      });
    },
    getTable: function(request) {
      return $http.get('api/table', {
        params: request
      });
    }, 
    getData: function(request) {
      return $http.get('api/data', {
        params: request
      });
    }
  } 
}]);
