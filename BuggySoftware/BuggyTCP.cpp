#include "BuggyTCP.hpp"
#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;
using logLevel = EmbeddedLogger::LogLevel;


TcpServer::TcpServer(int port): server(port), serverPort(port) {
  
}

void TcpServer::setup() {
  if (setupDone) return;
  logger << logLevel::DEBUG << "Starting Tcp Server" << EmbeddedLogger::endl;
  server.begin();
}

void TcpServer::sendPacket(const Packet packet) {
  char buffer[MAX_PACKET_LENGTH] = {0}; // Initialize buffer to 0
  PacketSerializer::serializePacket(packet, buffer);
  this->server.write(buffer, sizeof(packet)); // Cast to uint8_t pointer to access the byte data of the struct itself
  //this->server.write((uint8_t*)&packet, sizeof(packet)); // Cast to uint8_t pointer to access the byte data of the struct itself
}

Packet TcpServer::update() {
  WiFiClient client = this->server.available();   // listen for incoming clients
  Packet packet;
  packet.type = PacketType(0); // Will initialize packet type to an invalid packet that can be detected
  if (client) {
    delayMicroseconds(10);            // This is required for the Arduino Nano RP2040 Connect
    logger << logLevel::DEBUG << "CLIENT CONNECTED" << EmbeddedLogger::endl;
    if (client.available() >= 72) {
      logger << logLevel::DEBUG << "RECIEVED PACKAGE" << EmbeddedLogger::endl;
      client.readBytes(this->input_buffer, MAX_PACKET_LENGTH);
      logger << logLevel::DEBUG << "PACKAGE RECIEVED - DESERIALIZING" << EmbeddedLogger::endl;
      // deserialize packet
      memcpy(&packet, this->input_buffer, MAX_PACKET_LENGTH);
      //packet = PacketDeserializer::deserializePacket(this->input_buffer);
      logger << logLevel::DEBUG << "DESERIALIZED" << EmbeddedLogger::endl << "PACKET: " << String(packet.type).c_str() << EmbeddedLogger::endl;
    }
  }
  return packet;
}
