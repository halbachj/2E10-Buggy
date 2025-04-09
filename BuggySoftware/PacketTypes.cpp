#include "PacketTypes.hpp"



const char* getPacketName(PacketType type) {
  switch (type) {
    case PacketType::COMMAND:
      return " # COMMAND PACKET # ";
    case PacketType::CONTROL:
      return " # CONTROL PACKET # ";
    case PacketType::LOG:
      return " # LOG PACKET # ";
    case PacketType::STATUS:
      return " # STATUS PACKET # ";
    default:
      return " # UNKOWN PACKET # ";
  }

}