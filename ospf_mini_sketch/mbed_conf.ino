#include "mbed.h"

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
