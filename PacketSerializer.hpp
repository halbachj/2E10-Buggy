#ifndef PACKET_SERIALIZER_HPP
#define PACKET_SERIALIZER_HPP

#include "PacketTypes.hpp"

/*
const size_t MAX_PACKET_LENGTH = sizeof(Packet);

class PacketSerializer {
public:
  static void serializePacket(Packet packet, char buffer[MAX_PACKET_LENGTH]) {
    memcpy(buffer, &packet, MAX_PACKET_LENGTH);
  }
};
*/

class PacketDeserializer {
public:
  static Packet deserializePacket(char buffer[MAX_PACKET_LENGTH]) {
    PacketType type = PacketType(buffer[0]);
    Packet packet;
    packet.type = type;
    switch (type) {
      case PacketType::COMMAND:
        CommandPacket command;
        memcpy(&command, buffer, MAX_PACKET_LENGTH);
        packet.content.commandPacket = command;
        break;
      
      // NO use cases (Why would you send a log or status packet to the buggy???)
      case PacketType::LOG:
      case PacketType::STATUS:
      default:
        break;
    }

    return packet;
  }
};



#endif  //PACKET_SERIALIZER_HPP