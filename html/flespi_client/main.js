const hostname = "mqtt.flespi.io"
const port = 443
const path = ""
const clientId = "js-1"
const user = "3s897csODyMGcSwQ75LY7uTipFBIBnzsDvrBvHfZ6Pt6xQMsLnhGH0PVvetUrQcU"

var client = new Paho.Client(hostname, Number(port), clientId);

var messages = []
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
    handle_message(message.payloadString )
}

function onFail(context) {
    console.log("onFail");
}


function disconnect() {
    console.log("INFO", "Disconnecting from Server.");
    client.disconnect();
}



function handle_message(message){
    
    message = parse_message(message);
    messages.push(message)

    messages = moving_avg(5, messages)
    console.log(messages)
    document.getElementById('output').innerText += message + "\n";
    document.getElementById('output').scrollTop = document.getElementById('output').scrollHeight;

}

function parse_message(message){
    // console.log(message)
    return message.split(","); 
}

function moving_avg(num_messages, messages){
    if(messages.length > num_messages){
        messages = messages.slice(0, num_messages+1)
        return messages
    }
    return messages
}