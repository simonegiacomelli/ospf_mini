

#include <ArduinoBLE.h>

// BLE Battery Service
//BLEService batteryService("180F");
BLEService batteryService("1234");
// BLE Battery Level Characteristic
BLEUnsignedCharCharacteristic batteryLevelChar("2A19",  // standard 16-bit characteristic UUID
                                               BLERead |
                                               BLENotify); // remote clients will be able to get notifications if this characteristic changes

int counter = 0;  // last battery level reading from analog input
long previousMillis = 0;  // last time the battery level was checked, in ms

void setup() {
    Serial.begin(9600);    // initialize serial communication
    //while (!Serial);

    pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected

    // begin initialization
    if (!BLE.begin()) {
        Serial.println("starting BLE failed!");

        while (1);
    }

    auto name = "SHAZAM" + BLE.address();
    name.replace(":", "");
    BLE.setLocalName(name.c_str());

    BLE.setAdvertisedService(batteryService); // add the service UUID
    batteryService.addCharacteristic(batteryLevelChar); // add the battery level characteristic
    BLE.addService(batteryService); // Add the battery service
    batteryLevelChar.writeValue(counter); // set initial value for this characteristic

    //BLE.setAdvertisedServiceUuid("12340000-E8F2-537E-4F6C-D104768A1214");

    BLE.advertise();

    Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {

    BLE.advertise();

    // wait for a BLE central
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
                int next = ++counter;
                Serial.print("Connected to central: ");
                Serial.print(central.address());
                Serial.print(" sending value: ");
                Serial.println(next);
                batteryLevelChar.writeValue(next);
            }

        }
        // when the central disconnects, turn off the LED:
        digitalWrite(LED_BUILTIN, LOW);

    }
}