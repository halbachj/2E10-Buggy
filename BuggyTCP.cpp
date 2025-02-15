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

void TcpServer::sendAll(const Packet packet) {
  char buffer[MAX_PACKET_LENGTH] = {0};
  /*this->server.println("PACKET TYPE" + String(packet.type));
  this->server.println("PACKET CONTENT" + String(packet.content.logPacket.message));
  this->server.println("BUFFER SIZE: " + String(MAX_PACKET_LENGTH));
  for (const auto e : buffer) {
    this->server.println(String(int(e)));
  }
  
  this->server.println("SERIALIZED PACKET");
  for (const auto e : buffer) {
    this->server.println(String(int(e)));
  }*/
  PacketSerializer::serializePacket(packet, buffer);
  this->server.write(buffer, sizeof(packet));
}

void TcpServer::sendAll(const char* data, size_t count) {
  this->server.write(data, count);
}

void TcpServer::sendAll(const char* data) {
  this->server.write(data);
}


void TcpServer::sendPacket(Packet packet) {
  //this->server.write(PacketSerializer::serializePacket(packet), sizeof(packet));
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
