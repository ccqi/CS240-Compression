var express = require('express');
var app = express();
var bodyParser = require('body-parser');
var addon = require('./addon/build/Release/compression.node');
var mkdirp = require('mkdirp');
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
var rmdir = require( 'rmdir' );
var CronJob = require('cron').CronJob;
var purgeFiles = new CronJob('00 30 02 * * *', 
  function() {
    rmdir('app/files', function ( err, dirs, files ){
      console.log( dirs );
      console.log( files );
      console.log( 'all files are removed' );
    });
  },
  function() {
    console.log('cron job ended');
  }, 
  true
);

var encoder = new addon.TextWrapper();

app.post('/api/encode', function(req, res) {

  // save encoding to file
  var dir = 'app/files';
  var name = 'output';
  var extension = 'bin';
  //make directory if it doesn't already exist
  mkdirp(dir, function(err) {
    if (err) {
      console.error(err);
    } else {
      var timestamp = Math.floor(new Date() / 1000);     
      var filename = name + '_' + timestamp + '.' + extension;
      var path = dir + '/' + filename;
      var encoding = encoder.encode(path, req.body.data, req.body.method, req.body.max);
      console.log('New file: ' + filename + ' saved');
      res.setHeader('Content-Type', 'text/plain');
      res.send({
        'method': req.body.method,
        'filename': filename,
        'encoding': encoding
      });
    }
  });
});

/*app.post('/api', function(req, res) {
  var path = 'app/files/' + req.body.filename;
  var encoding = encoder.get(path, req.body.type, req.body.max);
  res.setHeader('Content-Type', 'application/json');
  res.send({
    'method': req.body.type,
    'encoding': encoding
  });
});*/


app.post('/api/table', function(req, res) {
  var path = 'app/files/' + req.body.filename;
  var table = encoder.getTable(path, req.body.type, req.body.start, req.body.increment)
  res.setHeader('Content-Type', 'application/json');
  res.send(table);
});

app.post('/api/data', function(req, res) {
  var path = 'app/files/' + req.body.filename;
  var data = encoder.getData(path, req.body.type, req.body.start, req.body.increment);
  res.setHeader('Content-Type', 'application/json');
  res.send(data);
});
