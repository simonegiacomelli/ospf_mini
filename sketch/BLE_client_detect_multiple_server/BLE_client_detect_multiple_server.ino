#ifdef ARDUINO_ARCH_MBED
#include "mbed.h"
#include <ArduinoBLE.h>
#include "ospf_mini_lib.h"
#else
//for CLion compilation
#include "../../src/ospf_mini_lib.h"
#include "../../ArduinoBLE/src/BLEDevice.h"
#include "../../ArduinoBLE/src/ArduinoBLE.h"

#endif
AntiSpin ap_loop(2000);

void setup() {
    ap_loop.setOnExecuteLambda([]() {
        printf("Main loop executing\n");
    });
    Serial.begin(9600);
    //while (!Serial);

    // begin initialization
    if (!BLE.begin()) {
        Serial.println("starting BLE failed!");

        while (1);
    }

    Serial.println("BLE Central - SensorTag button");
    Serial.println("Make sure to turn on the device.");

    // start scanning for peripheral
    BLE.scan();
}

void loop() {
    ap_loop.spin();
    // check if a peripheral has been discovered
    BLEDevice peripheral = BLE.available();

    if (peripheral) {
        // discovered a peripheral, print out address, local name, and advertised service
        Serial.print("Found v1.1 ");
        Serial.print(peripheral.address());
        Serial.print(" '");
        Serial.print(peripheral.localName());
        Serial.print("' ");
        Serial.print(peripheral.advertisedServiceUuid());
        Serial.println();

        // Check if the peripheral is a SensorTag, the local name will be:
        // "CC2650 SensorTag"

        if (peripheral.advertisedServiceUuid().equalsIgnoreCase("180F")) {
            // stop scanning
            BLE.stopScan();

            monitorSensorTagButtons(peripheral);
            printf("enabling scan\n");
            // peripheral disconnected, start scanning again
            BLE.scan();
        }
    }
}

void monitorSensorTagButtons(BLEDevice peripheral) {
    // connect to the peripheral
    Serial.println("Connecting ...");
    if (peripheral.connect()) {
        Serial.println("Connected");
    } else {
        Serial.println("Failed to connect!");
        return;
    }

    // discover peripheral attributes
    Serial.println("Discovering service 180F ...");
    if (peripheral.discoverService("180F")) {
        Serial.println("Service discovered");
    } else {
        Serial.println("Attribute discovery failed.");
        peripheral.disconnect();
        return;
    }

    // retrieve the simple key characteristic
    BLECharacteristic simpleKeyCharacteristic = peripheral.characteristic("2A19");

    // subscribe to the simple key characteristic
    Serial.println("Subscribing to simple key characteristic ...");
    if (!simpleKeyCharacteristic) {
        Serial.println("no simple key characteristic found!");
        peripheral.disconnect();
        return;
    } else if (!simpleKeyCharacteristic.canSubscribe()) {
        Serial.println("simple key characteristic is not subscribable!");
        peripheral.disconnect();
        return;
    } else if (!simpleKeyCharacteristic.subscribe()) {
        Serial.println("subscription failed!");
        peripheral.disconnect();
        return;
    } else {
        Serial.println("Subscribed");
        Serial.println("Press the right and left buttons on your SensorTag.");
    }
    int count = 0;
    auto enter_millis = millis();
    // while the peripheral is connected
    while (peripheral.connected() && count < 15) {
        auto delta = (millis() - enter_millis) / 1000;
        if (delta > 10) {
            printf("Timeout looping device. exiting loop\n");
            break;
        }

        // check if the value of the simple key characteristic has been updated
        if (simpleKeyCharacteristic.valueUpdated()) {
            // yes, get the value, characteristic is 1 byte so use byte value
            byte value = 0;
            count++;
            simpleKeyCharacteristic.readValue(value);

            Serial.print(peripheral.localName());
            Serial.print(",");
            Serial.print(value);
            Serial.print(",");
            Serial.print(peripheral.rssi());
            Serial.println("");
        }
    }
    peripheral.disconnect();

    Serial.println("SensorTag disconnected!");
}