#ifndef PACKET_FACTORY_HPP
#define PACKET_FACTORY_HPP

#include "PacketTypes.hpp"


class PacketFactory {
public:
  static Packet createStatusPacket(bool objectDetected, float distance, int leftMotorSpeed, int rightMotorSpeed, float leftMotorDistanceTraveled, float rightMotorDistanceTraveled, int leftIrSensorValue, int rightIrSensorValue) {
    StatusPacket status = {objectDetected, distance, leftMotorSpeed, rightMotorSpeed, leftMotorDistanceTraveled, rightMotorDistanceTraveled, leftIrSensorValue, rightIrSensorValue};
    Packet packet = {PacketType::STATUS, status};
    return packet;
  }
  static Packet createLogPacket(short count, const char* message) {
    LogPacket log = {count, ""};
    strncpy(log.message, message, MAX_MESSAGE_LENGTH);
    log.message[MAX_MESSAGE_LENGTH-1] = '\0'; // ADD NULL BYTE IN CASE MESSAGE WAS TO BIG

    Packet packet;
    packet.type = PacketType::LOG;
    packet.content.logPacket = log;
    return packet;
  }
};




#endif //PACKET_FACTORY_HPP