const hostname = "mqtt.flespi.io"
const port = 443
const path = ""
const clientId = "client_js-"
const user = "3s897csODyMGcSwQ75LY7uTipFBIBnzsDvrBvHfZ6Pt6xQMsLnhGH0PVvetUrQcU"
const num_messages = 5
var client = new Paho.Client(hostname, Number(port), clientId + Math.floor(Math.random() * 100000));

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
    console.log("onConnect");
    client.subscribe("cyber/rssi");
    print_output("Connected");
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
    handle_message(message.payloadString )
}

function onFail(context) {
    console.log("onFail");
}

function disconnect() {
    console.log("INFO", "Disconnecting from Server.");
    client.disconnect();
    print_output("Disconnected");
}

function handle_message(message){
    message = message.split(","); 
    console.log(message)
    print_output(message)

}

function print_output(data){
    document.getElementById('output').innerText += data + "\n";
    document.getElementById('output').scrollTop = document.getElementById('output').scrollHeight;
}