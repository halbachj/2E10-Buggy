#ifndef BUGGY_TCP_HPP
#define BUGGY_TCP_HPP

#include "BuggyWifi.hpp"
#include "PacketTypes.hpp"
#include "PacketSerializer.hpp"

/**
 * @class TcpServer
 * @brief The TCP server class provides functionality for setting up and managing a basic
 * TCP server using Wi-Fi.
 * @details It allows the server to listen for incoming client connections
 * on a specified port (default is 1234) and handle communication.
 * The class includes methods to update the server state, send data to all connected
 * clients, and handle client interactions.
 */
class TcpServer {
private:
  bool setupDone = false;
  const int serverPort;
  WiFiServer server;
  char input_buffer[MAX_PACKET_LENGTH];

public:
  /**
   * @brief Construct the TCP server on the specified port
   *
   * @param port the TCP port on which the server is supposed to run, defaults to 1234.
   */
  TcpServer(int port=1234);
  /**
   * @brief Setup method for the TCP server. needs to be called during setup
   */
  void setup();
  /**
   * @brief The update method handles the receiving of packets.
   * @details It will check if a client is connected and if enough
   * data is available and then deserialize the Packet and returning it.
   * 
   * @returns received Packet. Packet.type is 0 when no packet is received.
   */
  Packet update();
  /**
   * @brief Will send a packet to all clients connected to the server.
   */
  void sendPacket(const Packet packet);
};

#endif //BUGGY_TCP_HPP
