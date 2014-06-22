var websocket = null;
var _websocketKeepAlive;
var wsServerPort = -1;
var pendingWsMessageQueue = [];

var baseServerUrl = '10.1.10.161';


function game() {
    //var response = {};
    // response.control = "Flux Capacitor";
    // response.up = "Explode";
    // response.select = "Stun";
    // response.down = "Off";
    Pebble.sendAppMessage({
        "0": "Flux Capacitor",
        "1": "Explode",
        "2": "Stun",
        "3": "Off"
    });
}

function onWsMsg(evt) {
    var data = evt.data;
    var msg = JSON.parse(data);

    // This is something like {
    //     'category': 'SET_CONTROL',
    //     Matthew write more stuff here
    // }
}


Pebble.addEventListener("ready",
    function(e) {
        console.log("Hello world! - Sent from your javascript application.");
        setupWebsocket();
    }
);
Pebble.addEventListener("appmessage",
    function(e) {
        console.log("Received message: " + e.payload[1]);
        game();
    }
);

function sendObject(obj) {
    sendWsMessage(obj);
}


function onWsOpen(evt) {
    console.log("Websocket connection established");
    while (pendingWsMessageQueue.length > 0) {
        websocket.send(pendingWsMessageQueue.pop());
    }
}

function onWsClose(evt) {
    websocket = null;
}

function onWsError(evt) {
    websocket = null;
}

function sendWsMessage(msg) {
    if (typeof msg !== 'string') {
        msg = JSON.stringify(msg);
    }

    if (!websocket) {
        setupWebsocket();

        // We have to queue this message up to send because the socket isn't open yet
        pendingWsMessageQueue.push(msg);
    } else {
        websocket.send(msg);
    }
}


function setupWebsocket() {
    console.log ("setting up websocket");
    if (wsServerPort == -1) {
        requestPortFromServer('http://' + baseServerUrl + ':9000', function(port) {
            console.log('Port acquired from Spock: ', port);
            wsServerPort = port;
            createCommandSocket(wsServerPort);
        });
    } else {
        createCommandSocket(wsServerPort);
    }
}

function createCommandSocket(wsServerPort) {
    if (websocket !== null) {
        websocket.close();
    }

    websocket = new WebSocket('ws://' + baseServerUrl + ':' + wsServerPort);
    console.log('making websocket connection to ', 'ws://' + baseServerUrl + ':' + wsServerPort);
    websocket.onopen = onWsOpen;
    websocket.onclose = onWsClose;
    websocket.onmessage = onWsMsg;
    websocket.onerror = onWsError;

    if (_websocketKeepAlive) {
        clearInterval(_websocketKeepAlive);
    }

    _websocketKeepAlive = setInterval(keepWebsocketAlive, 1000);
}

function keepWebsocketAlive() {
    if (!websocket) {
        console.log("Reviving Websocket");
        setupWebsocket();
    }
}

function requestPortFromServer(serverUrl, callback) {
    console.log ('making http request to ' + serverUrl);
    var req = new XMLHttpRequest();
    req.open('GET', serverUrl, true);
    req.onload = function(e) {
        if (req.readyState == 4 && req.status == 200) {
            callback(req.responseText);
        } else {
            console.log('Error acquiring port from Spock');
        }
    };
    req.send(null);
}