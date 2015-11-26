var express = require('express');
var app = express();
var bodyParser = require('body-parser');
var addon = require('./addon/build/Release/compression.node');
var mkdirp = require('mkdirp');
var multer = require('multer');
var crypto = require('crypto');
var rmdir = require( 'rmdir' );
var config = require('./config');

app.use( bodyParser.json() );       // to support JSON-encoded bodies
app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
  extended: true
}));
app.use(express.static(__dirname + '/app'));
app.use('/tmp', express.static(__dirname + '/tmp'));
var upload = multer({dest: config.uploadPath});

app.get('/', function (req, res) {
  res.sendFile('app/index.html',{root:__dirname});
});

var server = app.listen(3000, function () {

  var host = server.address().address;
  var port = server.address().port;
  console.log('Compression app listening at http://%s:%s', host, port);

});

var CronJob = require('cron').CronJob;
var purgeFiles = new CronJob('00 30 02 * * *', 
  function() {
      rmdir(config.tmpDir, function ( err, dirs, files ){
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
  var dir = config.outputPath;
  var name = 'output';
  var extension = 'bin';

  //make directory if it doesn't already exist
  mkdirp(dir, function(err) {
    if (err) {
      console.error(err);
    } else {
      
      if (req.body.inputType != 'TEXT' && req.body.inputType != 'FILE')  {
        res.status(400).send('Input type not specified');
        return;
      }
      
      var timestamp = Math.floor(new Date() / 1000);     
      var filename = name + '_' + timestamp + '.' + extension;
      
      // create hash as filename
      var hash = crypto.createHash('md5').update(filename).digest('hex');
      
      var outputPath = dir + '/' + hash; 
      var content = req.body.content;
      if (req.body.inputType == 'FILE') {
        content =  config.uploadPath + '/' + req.body.content; 
      }
      var encoding = encoder.encode(outputPath, req.body.method, req.body.max, req.body.inputType, content);
      console.log('New file: ' + hash + ' saved');
      res.setHeader('Content-Type', 'text/plain');
      res.send({
        'method': req.body.method,
        'filename': hash,
        'encoding': encoding
      });
    }
  });
});

app.post('/api/table', function(req, res) {
  var encoder = new addon.TextWrapper();
  var path = config.outputPath + '/' + req.body.filename;
  var table = encoder.getTable(path, req.body.type, req.body.start, req.body.increment)
  res.setHeader('Content-Type', 'application/json');
  res.send(table);
});

app.post('/api/data', function(req, res) {
  var encoder = new addon.TextWrapper();
  var path = config.outputPath + '/' + req.body.filename;
  var data = encoder.getData(path, req.body.type, req.body.start, req.body.increment);
  res.setHeader('Content-Type', 'application/json');
  res.send(data);
});


app.post('/api/upload', upload.single('file'), function(req, res) {
  console.log('Uploaded new file: ' +  req.file.originalname);
  res.setHeader('Content-Type', 'application/json');
  res.status(200).send({'filename': req.file.filename});
});
