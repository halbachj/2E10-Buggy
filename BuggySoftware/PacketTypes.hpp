#ifndef PACKET_TYPES_HPP
#define PACKET_TYPES_HPP


enum PacketType {
  STATUS = 1, // Start at 1. Empty package will hold 0 here and can be detected
  LOG,
  COMMAND,
  CONTROL,   // USED FOR REMOTE CONTROL
};

/*
 * LOG PACKET
 */
const short MAX_MESSAGE_LENGTH = 64;
struct LogPacket {
  short count;
  char message[MAX_MESSAGE_LENGTH];
};


/*
 * STATUS PACKET
 */
struct StatusPacket {
  bool objectDetected;
  float objectDistance;
  int leftMotorSpeed;
  int rightMotorSpeed;
  float leftMotorDistanceTraveled;
  float rightMotorDistanceTraveled;
  int leftIrSensorValue;
  int rightIrSensorValue;
};


/*
 * COMMAND PACKET
 */
enum CommandType {
  STOP = 0,
  START
};

struct CommandPacket {
  CommandType type;
  uint16_t data;
};


/*
 * CONTROL PACKET
 */
struct ControlPacket {
  float x;
  float y;
};



/*
 * PACKET CONTAINER
 */
union PacketContent {
  StatusPacket statusPacket;
  CommandPacket commandPacket;
  LogPacket logPacket;
  ControlPacket controlPacket;
};

struct Packet {
  PacketType type;
  PacketContent content;
};


const size_t MAX_PACKET_LENGTH = sizeof(Packet);

#endif //PACKET_TYPES_HPP