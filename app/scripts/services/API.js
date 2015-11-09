angular.module('Compression').factory('C9nAPI', ['$http', function($http) {
  return {
    encode : function(request) {
      return $http.post('/api/encode',request);
    }
  } 
}]);
