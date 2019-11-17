//
// Created by Simone Giacomelli on 13.11.19.
//

#ifndef OSPF_MINI_LIB_OSPF_MINI_DEP_H
#define OSPF_MINI_LIB_OSPF_MINI_DEP_H

#ifdef ARDUINO_ARCH_MBED

#include <Arduino.h>
#include <ArduinoBLE.h>
#include "mbed.h"
#else
//for CLion compilation
#include "../../ArduinoBLE/src/BLEDevice.h"
#include "../../ArduinoBLE/src/ArduinoBLE.h"

#include <chrono>
#include <thread>
extern long startup_millis;

unsigned long millis() {
    using namespace std::chrono;
    auto res = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    if (startup_millis == 0)
        startup_millis = res;
    return res - startup_millis;
}

void delay(unsigned long ms){
    using namespace std;
    using namespace this_thread; // sleep_for, sleep_until
//    using namespace chrono; // nanoseconds, system_clock, seconds
        sleep_for(chrono::milliseconds(ms));
}
#endif

#endif //OSPF_MINI_LIB_OSPF_MINI_DEP_H
