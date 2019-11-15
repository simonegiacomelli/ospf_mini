#include "mbed.h"
#include <ArduinoBLE.h>
#include <list>
#include "ospf_mini_lib.h"

using namespace std;

struct OspfNanoBLE : OspfNano {

  void updateAdjacency() override {

  }
  void poll() override {
    BLE.poll();
  }
  void bleCentralDiscoverHandler(BLEDevice peripheral) {
    auto p = PeerDevice();
    p.address = peripheral.address().c_str();
    p.cost = -peripheral.rssi();
    p.millis = millis();
    linkState.adjacency.insert(p);
  }
};


OspfNanoBLE ospf;

void bleCentralDiscoverHandler(BLEDevice peripheral) {
  ospf.bleCentralDiscoverHandler(peripheral) ;
}

void setup() {
  Serial.begin(9600);
  //while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  Serial.println("BLE Central scan callback");

  // set the discovered event handle
  BLE.setEventHandler(BLEDiscovered, bleCentralDiscoverHandler);

  // start scanning for peripherals with duplicates
  BLE.scan(true);
  printf("Starting\n");
}


void loop() {
  ospf.handle();
}
