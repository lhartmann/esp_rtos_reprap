function EchoController($scope) {
	$scope.messageCount = 0;
	$scope.log = new Array();
	
	function logPrint(msg) {
		var entry = {
			time: Date(),
			text: msg
		};
		
		$scope.log.splice(0, 0, entry);
		
		if ($scope.log.length > 100)
			$scope.log.splice(100, $scope.log.length - 100);
	}
	
	logPrint("Started.");
	
	var ws = new WebSocket('ws://192.168.1.98/websocket/echo.cgi', ['soap', 'xmpp']);
	
	ws.onopen = function(e) {
		logPrint("Connection successful.");
		$scope.$apply();
	}

	ws.onerror = function(e) {
		logPrint("Connection failed.");
		$scope.$apply();
	}
	
	ws.onmessage = function(e) {
		$scope.messageCount++;
		logPrint("Got: "+e.data);
		$scope.$apply();
	}
	
	document.getElementById("frmMessage").onsubmit = function() {
		ws.send($scope.message);
		logPrint("Sent: "+$scope.message);
		$scope.message = "";
		$scope.$apply();
		return false;
	}
}
