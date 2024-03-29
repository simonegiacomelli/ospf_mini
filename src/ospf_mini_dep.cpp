//
// Created by Simone Giacomelli on 14.11.19.
//


#ifdef ARDUINO_ARCH_MBED
#include <Arduino.h>

#include "mbed.h"

//////////////////////////  printf redirection to serial START //////////////////////////
namespace mbed
{

class RedirSerial : public FileHandle {
  public:
    RedirSerial() {}
    ssize_t read(void *buffer, size_t size) override {
      return 0;
    }
    ssize_t write(const void *buffer, size_t size)  override {
      Serial.write((const uint8_t*)buffer, size);
      return size;
    }
    off_t seek(off_t offset, int whence = SEEK_SET) override {}
    int close() override {}
};

FileHandle *mbed_target_override_console(int)
{
  static RedirSerial res;
  return &res;
}

} //namespace mbed
//////////////////////////  printf redirection to serial END //////////////////////////
#else

#include <chrono>
#include <thread>

#include <sstream>
#include <string>
#include <fstream>
#include <list>

unsigned long startup_millis = 0;

#endif

using namespace std;

