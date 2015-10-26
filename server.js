var express = require('express');
var app = express();
var bodyParser = require('body-parser');
var addon = require('./addon/build/Release/compression.node');

app.use( bodyParser.json() );       // to support JSON-encoded bodies

app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
  extended: true
}));

app.use(express.static(__dirname + '/app'));

app.get('/', function (req, res) {
  res.sendFile('app/index.html',{root:__dirname});
});

var server = app.listen(3000, function () {

  var host = server.address().address;
  var port = server.address().port;

  console.log('Example app listening at http://localhost:3000...');

});

app.post('/api/encode', function(req, res) {
  var textWrapper = new addon.TextWrapper(req.body.data);
  var encoding = textWrapper.encode(req.body.data, req.body.method);
  console.log(encoding);
  res.setHeader('Content-Type', 'application/json');
  res.send({
    'method': req.body.method,
    'encoding': encoding
  });
});
