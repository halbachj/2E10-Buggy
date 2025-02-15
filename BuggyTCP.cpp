#include "BuggyTCP.hpp"
#include "Logger.hpp"

using LogLevel = mcu::Logger::LogLevel;


TcpServer::TcpServer(int port): server(port), serverPort(port) {
  
}

void TcpServer::setup() {
  if (setupDone) return;
  mcu::logger << "Starting Tcp Server" << mcu::LeanStreamIO::endl;
  server.begin();
}

void TcpServer::sendPacket(const Packet packet) {
  uint8_t buffer[MAX_PACKET_LENGTH] = {0}; // Initialize buffer to 0
  this->server.write((uint8_t*)&packet, sizeof(packet)); // Cast to uint8_t pointer to access the byte data of the struct itself
}

Packet TcpServer::update() {
  WiFiClient client = this->server.available();   // listen for incoming clients
  Packet packet;
  if (client) {
    delayMicroseconds(10);            // This is required for the Arduino Nano RP2040 Connect
    //mcu::logger << "CLIENT CONNECTED" << mcu::LeanStreamIO::endl;
    if (client.available()) {
      mcu::logger << "RECIEVED PACKAGE" << mcu::LeanStreamIO::endl;
      client.readBytes(this->input_buffer, MAX_PACKET_LENGTH);
      mcu::logger << "PACKAGE RECIEVED - DESERIALIZING" << mcu::LeanStreamIO::endl;
      // deserialize packet
      packet = PacketDeserializer::deserializePacket(this->input_buffer);
      mcu::logger << "DESERIALIZED" << mcu::LeanStreamIO::endl << "PACKET: " << String(packet.type).c_str() << mcu::LeanStreamIO::endl;
    }
  }
  return packet;
}
