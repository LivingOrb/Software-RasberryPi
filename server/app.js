'use strict';

var async = require('async');
var bodyParser = require('body-parser');
var config = require('config-path')();
var execFile = require('child_process').execFile;
var express = require('express');
var fs = require('fs');
var path = require('path');

var process;

function api() {
	var router = express.Router();

	router.get('/modes', function(req, res, next) {
		return fs.readdir(config.modesPath, function(err, files) {
			if (err) return next(err);

			return async.map(files, function(filename, callback) {
				return fs.readFile(path.join(config.modesPath, filename), function(err, data) {
					if (err) return callback(err);
					return callback(null, {
						name: path.basename(filename, path.extname(filename)),
						script: data.toString(),
					});
				});
			}, function(err, modes) {
				if (err) return next(err);
				return res.json({
					modes: modes,
				});
			});
		});
	});

	router.put('/modes/:modeName', function(req, res, next) {
		if (!req.params.modeName || !req.body.script)
			return res.sendStatus(400);

		var filename = req.params.modeName + '.lua';
		return fs.writeFile(path.join(config.modesPath, filename), req.body.script, function(err) {
			if (err) return next(err);
			return res.sendStatus(200);
		});
	});

	router.post('/start-mode', function(req, res, next) {
		if (!req.body.script)
			return res.sendStatus(400);

		if (process)
			process.kill();

		process = execFile(config.gamePath, {
			cwd: config.gameWd,
			maxBuffer: config.gameMaxBuffer,
		}, function(err) {
			if (err) console.error(err);
		});

		process.stdout.pipe(fs.createWriteStream(path.join(config.publicPath, 'stdout.log')));
		process.stderr.pipe(fs.createWriteStream(path.join(config.publicPath, 'stderr.log')));

		process.stdin.write(req.body.script, 'utf8');
		process.stdin.end();

		return res.sendStatus(200);
	});

	router.post('/stop-mode', function(req, res, next) {
		if (!process)
			return res.sendStatus(404);

		process.kill();
		process = null;
		return res.sendStatus(200);
	});

	return router;
}

module.exports = function(callback) {
	var app = express();
	app.use(bodyParser.json());

	app.use('/api', api());
	app.use(express.static(config.publicPath));

	return callback(null, app);
};

if (require.main === module)
	module.exports(function(err, app) {
		if (err) throw err;
		return app.listen(config.port, function() {
			console.log('Listening on port %d', config.port);
		});
	});
