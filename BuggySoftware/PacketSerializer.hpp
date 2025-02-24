#ifndef PACKET_SERIALIZER_HPP
#define PACKET_SERIALIZER_HPP

#include "PacketTypes.hpp"


const size_t MAX_PACKET_LENGTH = sizeof(Packet);

class PacketSerializer {
public:
  static void serializePacket(Packet packet, char buffer[MAX_PACKET_LENGTH]) {
    memcpy(buffer, &packet, MAX_PACKET_LENGTH);
  }
};

class PacketDeserializer {
public:
  static Packet deserializePacket(char buffer[MAX_PACKET_LENGTH]) {
    PacketType type = PacketType(buffer[0]);
    Packet packet;

    return packet;
  }
};



#endif  //PACKET_SERIALIZER_HPP