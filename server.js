var express = require('express');
var app = express();
var bodyParser = require('body-parser');
var addon = require('./addon/build/Release/compression.node');
var mkdirp = require('mkdirp');
var multer = require('multer');
app.use( bodyParser.json() );       // to support JSON-encoded bodies

app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
  extended: true
}));

app.use(express.static(__dirname + '/app'));
app.use(express.static(__dirname + '/node_modules'));

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


app.post('/api/encode', function(req, res) {

  // save encoding to file
  var encoder = new addon.TextWrapper();
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
      var outputPath = dir + '/' + filename;
      if (req.body.inputType != 'TEXT' && req.body.inputType != 'FILE')  {
        res.status(400).send('Input type not specified');
        return;
      }

      var encoding = encoder.encode(outputPath, req.body.method, req.body.max, req.body.inputType, req.body.content);
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

app.post('/api/table', function(req, res) {
  var encoder = new addon.TextWrapper();
  var path = 'app/files/' + req.body.filename;
  var table = encoder.getTable(path, req.body.type, req.body.start, req.body.increment)
  res.setHeader('Content-Type', 'application/json');
  res.send(table);
});

app.post('/api/data', function(req, res) {
  var encoder = new addon.TextWrapper();
  var path = 'app/files/' + req.body.filename;
  var data = encoder.getData(path, req.body.type, req.body.start, req.body.increment);
  res.setHeader('Content-Type', 'application/json');
  res.send(data);
});

var upload = multer({dest: 'uploads/'});

app.post('/api/upload', upload.single('file'), function(req, res) {
  console.log('Uploaded new file: ' +  req.file.originalname);
  res.setHeader('Content-Type', 'application/json');
  res.status(200).send({'filename': req.file.filename});
});



