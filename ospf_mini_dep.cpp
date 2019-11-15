//
// Created by Simone Giacomelli on 14.11.19.
//


#ifdef ARDUINO_ARCH_MBED
#include <Arduino.h>
#else

#include <chrono>
#include <thread>

#include <sstream>
#include <string>
#include <fstream>
#include <list>

unsigned long startup_millis = -1;

#endif

using namespace std;

