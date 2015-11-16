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

var encoder = new addon.TextWrapper();

app.post('/api/encode', function(req, res) {
  encoder.set(req.body.data);
  encoder.encode(req.body.method);
  res.send('sucessfully encoded message');
});

app.get('/api', function(req, res) {
  var encoding = encoder.get(req.query.type, req.query.max);
  res.setHeader('Content-Type', 'application/json');
  res.send({
    'method': req.query.type,
    'encoding': encoding
  });
});

app.get('/api/table', function(req, res) {
  var table = encoder.getTable(req.query.type);
  res.setHeader('Content-Type', 'application/json');
  res.send(table);
});

app.get('/api/data', function(req, res) {
  var data = encoder.getData(req.query.type);
  res.setHeader('Content-Type', 'application/json');
  res.send(data);
});
