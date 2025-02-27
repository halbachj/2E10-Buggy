#ifndef PACKET_FACTORY_HPP
#define PACKET_FACTORY_HPP

#include "PacketTypes.hpp"

/**
 * @class PacketFactory
 * @brief Factory class for creating different types of packets.
 *
 * Provides static methods to generate status and log packets used in communication.
 */
class PacketFactory {
public:
  /**
   * @brief Creates a status packet containing sensor and motor data.
   * @param objectDetected Boolean indicating if an object is detected.
   * @param distance Distance to the detected object.
   * @param leftMotorSpeed Speed of the left motor.
   * @param rightMotorSpeed Speed of the right motor.
   * @param leftMotorDistanceTraveled Distance traveled by the left motor.
   * @param rightMotorDistanceTraveled Distance traveled by the right motor.
   * @param leftIrSensorValue IR sensor reading on the left side.
   * @param rightIrSensorValue IR sensor reading on the right side.
   * @return A Packet object containing the status information.
   */
  static Packet createStatusPacket(bool objectDetected, float distance, int leftMotorSpeed, int rightMotorSpeed, float leftMotorDistanceTraveled, float rightMotorDistanceTraveled, int leftIrSensorValue, int rightIrSensorValue) {
    StatusPacket status = {objectDetected, distance, leftMotorSpeed, rightMotorSpeed, leftMotorDistanceTraveled, rightMotorDistanceTraveled, leftIrSensorValue, rightIrSensorValue};
    Packet packet = {PacketType::STATUS, status};
    return packet;
  }

  /**
   * @brief Creates a log packet containing a log message.
   * @param count Number of times this message has been logged.
   * @param message Log message content.
   * @return A Packet object containing the log message.
   */
  static Packet createLogPacket(short count, const char* message) {
    LogPacket log = {count, ""};
    strncpy(log.message, message, MAX_MESSAGE_LENGTH);
    log.message[MAX_MESSAGE_LENGTH-1] = '\0'; // Ensure null termination in case message is too long

    Packet packet;
    packet.type = PacketType::LOG;
    packet.content.logPacket = log;
    return packet;
  }
};




#endif //PACKET_FACTORY_HPP