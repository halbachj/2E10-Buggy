#ifndef SERIALWIRELESS_LOGGER_INTERFACE_HPP
#define SERIALWIRELESS_LOGGER_INTERFACE_HPP


#include "SerialLogger.hpp"
#include "WirelessLogger.hpp"
#include "Arduino.h"
#include "BuggyTCP.hpp"
#include "PacketFactory.hpp"

namespace EmbeddedLogger {

class SerialWirelessLogger : public SerialLogger, public WirelessLogger {
private:
public:
  SerialWirelessLogger(TcpServer& server) : WirelessLogger(server) {};
  void write(const char* data, size_t length) override {
    SerialLogger::write(data, length);
    WirelessLogger::write(data, length);
  }
};

}


#endif //SERIALWIRELESS_LOGGER_INTERFACE_HPP