
function game(){
  //var response = {};
  // response.control = "Flux Capacitor";
  // response.up = "Explode";
  // response.select = "Stun";
  // response.down = "Off";
  Pebble.sendAppMessage({"0": "Flux Capacitor", "1": "Explode", "2": "Stun", "3": "Off"});
}



Pebble.addEventListener("ready",
    function(e) {
        console.log("Hello world! - Sent from your javascript application.");
    }
);
Pebble.addEventListener("appmessage", 
	function(e){
		console.log("Received message: " + e.payload[1]);
    game();
	}
);