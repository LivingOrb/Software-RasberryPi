angular.module('orb', [])

.controller('OrbCtrl', function($http, $scope) {
	'use strict';

	var editor = ace.edit('editor');
	editor.setTheme('ace/theme/monokai');
	editor.getSession().setMode('ace/mode/lua');

	var modeName;

	$scope.modes = [];

	function save() {
		if (!modeName)
			return saveAs();

		var script = editor.getValue();
		$http.put('/api/modes/' + modeName, {
			script: script,
		}).then(function() {
			if (!$scope.modes.some(function(mode) {
				if (mode.name === modeName) {
					mode.script = script;
					return true;
				}
			})) {
				$scope.modes.push({
					name: modeName,
					script: script,
				});
			}
		});
	}

	function saveAs() {
		modeName = prompt('Name:', modeName || 'new-mode');
		if (modeName)
			return save();
	}

	function startMode() {
		$http.post('/api/start-mode', {
			script: editor.getValue(),
		});
	}

	editor.commands.addCommand({
		name: 'save',
		bindKey: {
			win: 'Ctrl-S',
			mac: 'Command-S',
		},
		exec: save,
	});

	editor.commands.addCommand({
		name: 'saveAs',
		bindKey: {
			win: 'Ctrl-Shift-S',
			mac: 'Command-Shift-S',
		},
		exec: saveAs,
	});

	editor.commands.addCommand({
		name: 'startMode',
		bindKey: {
			win: 'Ctrl-Enter',
			mac: 'Command-Enter',
		},
		exec: startMode,
	});

	$scope.save = save;
	$scope.saveAs = saveAs;
	$scope.startMode = startMode;

	$http.get('/api/modes')
		.then(function(response) {
			$scope.modes = response.data.modes;
		});

	$scope.stopMode = function(script) {
		$http.post('/api/stop-mode');
	};

	$scope.showScript = function(mode) {
		modeName = mode.name;
		editor.setValue(mode.script);
	};
});
