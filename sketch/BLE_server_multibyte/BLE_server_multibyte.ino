

#ifdef ARDUINO_ARCH_MBED
#include "ospf_mini_lib.h"
#else
//for CLion compilation
#include "../../src/ospf_mini_lib.h"
#include "../../src/BLEService.h"

#endif
#define MAX_LEN 4


// BLE Battery Service
BLEService batteryService("180F");
//BLEService batteryService("1234");
// BLE Battery Level Characteristic
BLECharacteristic batteryLevelChar("2A19",  // standard 16-bit characteristic UUID
                                   BLEWrite |
                                   BLERead |
                                   BLENotify, MAX_LEN,
                                   false); // remote clients will be able to get notifications if this characteristic changes


char buffer[MAX_LEN];

char counter = 0;  // last battery level reading from analog input
long previousMillis = 0;  // last time the battery level was checked, in ms
AntiSpin ap_loop(2000);

void setup() {

    ap_loop.setOnExecuteLambda([]() {
        printf("Main loop executing\n");
    });

    Serial.begin(9600);    // initialize serial communication
    while (!Serial);
//    BLE.debug(Serial);

    pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected


    // begin initialization
    if (!BLE.begin()) {
        Serial.println("starting BLE failed!");

        while (1);
    }

    static auto name = "SHAZAM" + BLE.address();
    name.replace(":", "");
    BLE.setLocalName(name.c_str());

    BLE.setAdvertisedService(batteryService); // add the service UUID
    batteryService.addCharacteristic(batteryLevelChar); // add the battery level characteristic
    BLE.addService(batteryService); // Add the battery service

    //BLE.setAdvertisedServiceUuid("12340000-E8F2-537E-4F6C-D104768A1214");

    BLE.advertise();

    Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
    ap_loop.spin();
//    delay(350);
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
                Serial.print("Connected to central: ");
                Serial.print(central.address());
                Serial.print(" sending value: ");
                for (int idx = 0; idx < MAX_LEN; idx++) {
                    char val = counter;
                    Serial.print(val);
                    Serial.print(" ");
                    buffer[idx] = val;
                    counter++;
                }
                batteryLevelChar.writeValue((char*)buffer, MAX_LEN);
                Serial.println(". Value sent");
            }

        }
        // when the central disconnects, turn off the LED:
        digitalWrite(LED_BUILTIN, LOW);

    }
}