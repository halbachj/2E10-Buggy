#ifndef BUGGY_TCP_HPP
#define BUGGY_TCP_HPP

#include "BuggyWifi.hpp"
#include "PacketTypes.hpp"
#include "PacketSerializer.hpp"

/*
 * TCP SERVER CLASS
 * The TCP server class provides functionality for setting up and managing a basic
 * TCP server using Wi-Fi. It allows the server to listen for incoming client connections
 * on a specified port (default is 1234) and handle communication with up to 3 clients.
 * The class includes methods to update the server state, send data to all connected
 * clients, and handle individual client interactions. Clients are stored in an array,
 * and the server ensures that communication is properly managed for each active client.
 */
class TcpServer {
private:
  bool setupDone = false;
  const int serverPort;
  WiFiServer server;
  char input_buffer[MAX_PACKET_LENGTH];

public:
  TcpServer(int port=1234);
  void setup();
  Packet update();
  void sendPacket(Packet packet);
  void sendAll(Packet packet);
  void sendAll(const char* data);
  void sendAll(const char* data, size_t count);
};

#endif //BUGGY_TCP_HPP
