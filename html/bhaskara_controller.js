function BhaskaraController($scope) {
	$scope.firstValidRoot = 0;
	$scope.secondValidRoot = 0;

	console.log("BHWS: Starting setup...");
	
	var ws = new WebSocket('ws://192.168.1.98/websocket/bhaskara.cgi', ['soap', 'xmpp']);
	
	ws.onopen = function(e) {
	}

	ws.onerror = function(e) {
	}
	
	ws.onmessage = function(e) {
		console.log("BHWS: Got response: ", e);
		var data = JSON.parse(e.data);
		
		if (data && data.isComplex) {
			$scope.firstValidRoot  = data.real + "+" + data.imag + "i";
			$scope.secondValidRoot = data.real + "-" + data.imag + "i";
		} else {
			$scope.firstValidRoot  = data.x1;
			$scope.secondValidRoot = data.x2;
		}
				
		$scope.$apply();
	}
	
	document.getElementById("frmBhaskara").addEventListener("submit", function() {
		console.log("BHWS: Preparing to send data...");
		var msg = {
			a: $scope.a,
			b: $scope.b,
			c: $scope.c
		};
		console.log("BHWS: Sending data: ", msg);
		ws.send(JSON.stringify(msg));
		return true;
	});
	
	document.getElementById("btnTest").addEventListener("click", function(e) {
		console.log("$scope.firstValidRoot  = ", $scope.firstValidRoot);
		console.log("$scope.secondValidRoot = ", $scope.secondValidRoot);
	});
	
	console.log("BHWS: Setup completed.");
}
