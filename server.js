var express = require('express');
var app = express();
var bodyParser = require('body-parser');
var addon = require('./addon/build/Release/compression.node');
app.use( bodyParser.json() );       // to support JSON-encoded bodies
app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
  extended: true
})); 
app.use(express.static(__dirname + '/static'));
app.get('/', function (req, res) {
  res.sendfile('static/index.html',{root:__dirname});
});

var server = app.listen(3000, function () {

  var host = server.address().address;
  var port = server.address().port;

  console.log('Example app listening at http://localhost:3000...');

});

app.post('/encode', function(req, res) {
  console.log(addon.hello());
  res.send('You sent the message "' + req.body.message + '".');
});
