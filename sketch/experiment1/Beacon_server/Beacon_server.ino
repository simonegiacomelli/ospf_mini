
//#include <ArduinoBLE.h>
#include <ospf_mini_lib.h>

BLEService ledService("180F"); // create service

// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic switchCharacteristic("2A19", BLERead | BLEWrite | BLENotify);

const int ledPin = LED_BUILTIN; // pin to use for the LED
AntiSpin ap_loop(2000);

void setup() {
    ap_loop.setOnExecuteLambda([]() {
        printf("Main loop executing %ul\r\n", ap_loop.spin_counter);
    });
    Serial.begin(9600);
//    while (!Serial);

    pinMode(ledPin, OUTPUT); // use the LED pin as an output

    // begin initialization
    if (!BLE.begin()) {
        Serial.println("starting BLE failed!");

        while (1);
    }
    Serial.print("DM_CONN_MAX: ");
    Serial.println(DM_CONN_MAX);

    static auto name = "SHAZAM" + BLE.address();
    name.replace(":", "");
    BLE.setLocalName(name.c_str());

    // set the UUID for the service this peripheral advertises
    BLE.setAdvertisedService(ledService);

    // add the characteristic to the service
    ledService.addCharacteristic(switchCharacteristic);

    // add service
    BLE.addService(ledService);

    // assign event handlers for connected, disconnected to peripheral
    BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
    BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

    // assign event handlers for characteristic
    switchCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);
    // set an initial value for the characteristic
    switchCharacteristic.setValue(0);

    // start advertising
    BLE.advertise();

    Serial.println(("Bluetooth device active, waiting for connections..."));
}

bool connectionReceived = false;
long previousMillis = 0;
int counter = 0;

void loop() {
    // poll for BLE events
    ap_loop.spin();
    BLE.poll();
//    if (connectionReceived) {
//        connectionReceived = false;
//        Serial.print("Setting on advertisment: ");
//        Serial.println(BLE.advertise());
//    }

    BLEDevice central = BLE.central();

    if (central) {

        // turn on the LED to indicate the connection:
        digitalWrite(LED_BUILTIN, HIGH);

        // if one central is connected:
        if (central.connected()) {
            long currentMillis = millis();
            // if x ms have passed, check the battery level:
            if (currentMillis - previousMillis >= 500) {
                previousMillis = currentMillis;
                if (counter == 100)
                    counter = 0;
                //int next = ++counter;
                int next = -central.rssi();
                Serial.print("Connected to central: ");
                Serial.print(central.address());
                Serial.print(" sending value: ");
                Serial.print(next);
                switchCharacteristic.writeValue(next);
                Serial.println(". Value sent");
            }

        }
        // when the central disconnects, turn off the LED:
        digitalWrite(LED_BUILTIN, LOW);
    }
}

void blePeripheralConnectHandler(BLEDevice central) {
    // central connected event handler
    Serial.print("Connected event, central: ");
    Serial.println(central.address());
    connectionReceived = true;
}

void blePeripheralDisconnectHandler(BLEDevice central) {
    // central disconnected event handler
    Serial.print("Disconnected event, central: ");
    Serial.println(central.address());
}

void switchCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
    // central wrote new value to characteristic, update LED
    Serial.print("Characteristic event, written: ");
    Serial.println(switchCharacteristic.value());
}