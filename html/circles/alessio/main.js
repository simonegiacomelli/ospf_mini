let defaultConf = {
    hostname: "mqtt.flespi.io",
    port: 443,
    path: "",
    clientId: "client_js-" + Math.floor(Math.random() * 100000),
    user: "3s897csODyMGcSwQ75LY7uTipFBIBnzsDvrBvHfZ6Pt6xQMsLnhGH0PVvetUrQcU",
    pass: "",
    num_messages: 5
};

class MqttClient {
    constructor(conf) {
        bind(this);
        this.onConnected = 0;
        this.onMessage = 0;

        if (conf === undefined)
            conf = defaultConf;
        this.conf = conf;
        this.client = new Paho.Client(conf.hostname, Number(conf.port), conf.clientId);
    }

    bind(caller) {
        for (const key of Object.getOwnPropertyNames(caller.constructor.prototype)) {
            const value = caller[key];

            if (key !== 'constructor' && typeof value === 'function')
                caller[key] = value.bind(caller);
        }
    }

    connect() {
        console.log("Init")

        this.client.onMessageArrived = this.onMessageArrived;
        this.client.onConnectionLost = this.onConnectionLost;

        let options = {
            // invocationContext: { host: hostname, port: port, path: client.path, clientId: clientId },
            timeout: 3,
            // keepAliveInterval: 60,
            // cleanSession: cleanSession,
            useSSL: true,
            reconnect: true,
            onSuccess: this.onConnect,
            userName: this.conf.user,
            password: this.conf.pass,
            onFailure: this.onFail
        };

        this.client.connect(options);
    }

    onConnect(context) {
        console.log("onConnect");
        this.client.subscribe("cyber/rssi");
        if (this.onConnected !== 0)
            this.onConnected();
    }

    onConnectionLost(responseObject) {
        if (responseObject.errorCode !== 0) {
            console.log("onConnectionLost:" + responseObject.errorMessage);
        }
    }

    onMessageArrived(message) {

        if (this.onMessage !== 0)
            this.onMessage(message.topic, message.payloadString);
        else
            console.log("onMessageArrived:" + message.payloadString);
    }

    onFail(context) {
        console.log("onFail");
    }

    disconnect() {
        console.log("INFO", "Disconnecting from Server.");
        this.client.disconnect();
    }

}
