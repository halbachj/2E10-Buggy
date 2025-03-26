#include "UdpServer.hpp"
#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;
using logLevel = EmbeddedLogger::LogLevel;


UdpServer::UdpServer(int port): serverPort(port) {
  
}

void UdpServer::setup(IPAddress broadcastAddress) {
  if (setupDone) return;
  logger << logLevel::DEBUG << F("Starting Udp Server") << EmbeddedLogger::endl;
  this->broadcastAddress = broadcastAddress;
  server.begin(this->serverPort);
}

void UdpServer::sendPacket(const Packet packet) {
  char buffer[MAX_PACKET_LENGTH] = {0}; // Initialize buffer to 0
  PacketSerializer::serializePacket(packet, buffer);
  this->server.beginPacket(this->broadcastAddress, this->serverPort);
  this->server.write(buffer, sizeof(packet));
  this->server.endPacket();
}

Packet UdpServer::update() {
  Packet packet;
  unsigned int udpPacketSize;
  packet.type = PacketType(0); // Will initialize packet type to an invalid packet that can be detected
  logger << logLevel::DEBUG << F("CLIENT CONNECTED") << EmbeddedLogger::endl;
  if (this->server.available() >= 72) {
    logger << logLevel::DEBUG << F("RECIEVED UDP PACKAGE") << EmbeddedLogger::endl;
    udpPacketSize = this->server.parsePacket();
    if (udpPacketSize >= 72) {
      this->server.readBytes(this->input_buffer, MAX_PACKET_LENGTH);
      logger << logLevel::DEBUG << F("UDP PACKAGE RECIEVED - DESERIALIZING") << EmbeddedLogger::endl;
      // deserialize packet
      memcpy(&packet, this->input_buffer, MAX_PACKET_LENGTH);
      //packet = PacketDeserializer::deserializePacket(this->input_buffer);
      logger << logLevel::DEBUG << F("DESERIALIZED UDP ") << EmbeddedLogger::endl << F("PACKET: ") << String(packet.type).c_str() << EmbeddedLogger::endl;
    }
  }
  return packet;
}
