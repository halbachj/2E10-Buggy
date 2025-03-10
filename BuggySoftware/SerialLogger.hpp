#ifndef SERIAL_LOGGER_INTERFACE_HPP
#define SERIAL_LOGGER_INTERFACE_HPP


#include "EmbeddedLogger.hpp"
#include "Arduino.h"

namespace EmbeddedLogger {

class SerialLogger : virtual public ILogTarget {
private:
public:
  SerialLogger() = default;
  void write(const char* data, size_t length) override {
    Serial.write(data, length);
  }
};

}



#endif //SERIAL_LOGGER_INTERFACE_HPP