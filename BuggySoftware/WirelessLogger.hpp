#ifndef WIRELESS_LOGGER_INTERFACE_HPP
#define WIRELESS_LOGGER_INTERFACE_HPP


#include "EmbeddedLogger.hpp"
#include "Arduino.h"
#include "BuggyTCP.hpp"
#include "PacketFactory.hpp"

namespace EmbeddedLogger {

class WirelessLogger : virtual public ILogTarget {
private:
  TcpServer& server;
public:
  WirelessLogger(TcpServer server) : server(server) {};
  void write(const char* data, size_t length) override {
    Packet packet = PacketFactory::createLogPacket(length, data);
    server.sendPacket(packet);
  }
};

}

#endif //WIRELESS_LOGGER_INTERFACE_HPP