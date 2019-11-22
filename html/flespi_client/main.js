const hostname = "mqtt.flespi.io"
const port = 443
const path = ""
const clientId = "js-1"
const user = "3s897csODyMGcSwQ75LY7uTipFBIBnzsDvrBvHfZ6Pt6xQMsLnhGH0PVvetUrQcU"

var client = new Paho.Client(hostname, Number(port), clientId);

function connect(){
    console.log("Init")
    console.log(client);

    client.onMessageArrived = onMessageArrived;
    client.onConnectionLost = onConnectionLost;

    var options = {
        // invocationContext: { host: hostname, port: port, path: client.path, clientId: clientId },
        timeout: 3,
        // keepAliveInterval: 60,
        // cleanSession: cleanSession,
        useSSL: true,
        reconnect: true,
        onSuccess: onConnect,
        userName: user,
        onFailure: onFail
      };

    client.connect(options);
}

function onConnect(context) {
    // Once a connection has been made, make a subscription and send a message.
    console.log("onConnect");
    client.subscribe("cyber/rssi");
    // message = new Paho.MQTT.Message("Hello");
    // message.destinationName = "World";
    // client.send(message);
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
    if (responseObject.errorCode !== 0) {
        console.log("onConnectionLost:"+responseObject.errorMessage);
    }
}

// called when a message arrives
function onMessageArrived(message) {
    console.log("onMessageArrived:"+message.payloadString);
}

function onFail(context) {
    console.log("onFail");
}


function disconnect() {
    console.log("INFO", "Disconnecting from Server.");
    client.disconnect();
}