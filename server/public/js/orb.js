angular.module('orb', [])

.controller('OrbCtrl', function($http, $scope) {
	'use strict';

	var editor = ace.edit('editor');
	editor.setTheme('ace/theme/monokai');
	editor.getSession().setMode('ace/mode/lua');

	$scope.modes = [];

	$http.get('/api/modes')
		.then(function(response) {
			$scope.modes = response.data.modes;
		});

	$scope.startMode = function(script) {
		$http.post('/api/start-mode', {
			script: editor.getValue(),
		});
	};

	$scope.stopMode = function(script) {
		$http.post('/api/stop-mode');
	};

	$scope.showScript = function(script) {
		editor.setValue(script);
	};
});
