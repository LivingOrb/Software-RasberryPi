angular.module('orb', [])

.controller('OrbCtrl', function($http, $scope) {
	'use strict';

	$scope.modes = [];
	$scope.script = 'function update(dt)\nend';

	$http.get('/api/modes')
		.then(function(response) {
			$scope.modes = response.data.modes;
		});

	$scope.startMode = function(script) {
		$http.post('/api/start-mode', {
			script: script,
		});
	};

	$scope.stopMode = function(script) {
		$http.post('/api/stop-mode');
	};

	$scope.showScript = function(script) {
		$scope.script = script;
	};
});
