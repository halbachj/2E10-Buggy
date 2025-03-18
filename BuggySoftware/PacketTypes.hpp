#ifndef PACKET_TYPES_HPP
#define PACKET_TYPES_HPP

/**
 * @enum PacketType
 * @brief Defines different types of packets used for communication.
 */
enum PacketType {
    // Start at one so empty packages can be detected with 0 as the Packet type
    STATUS = 1, ///< Status packet containing sensor and motor data.
    LOG,        ///< Log packet containing debug messages.
    COMMAND,    ///< Command packet for controlling the system.
    CONTROL     ///< Control packet for remote controlling the buggy.
};


/**
 * @brief Maximum length for log messages.
 */
const short MAX_MESSAGE_LENGTH = 64; 

/**
 * @struct LogPacket
 * @brief Represents a log message packet.
 */
struct LogPacket {
  short count;
  char message[MAX_MESSAGE_LENGTH];
};


/**
 * @struct StatusPacket
 * @brief Represents a status update packet containing sensor and motor data.
 */
struct StatusPacket {
  bool objectDetected;              ///< Indicates if an object is detected.
  float objectDistance;             ///< Distance to the detected object.
  int leftMotorSpeed;               ///< Speed of the left motor.
  int rightMotorSpeed;              ///< Speed of the right motor.
  float leftMotorDistanceTraveled;  ///< Distance traveled by the left motor.
  float rightMotorDistanceTraveled; ///< Distance traveled by the right motor.
  int leftIrSensorValue;            ///< IR sensor reading on the left side.
  int rightIrSensorValue;           ///< IR sensor reading on the right side.
};


/**
 * @enum CommandType
 * @brief Defines possible command types for the system.
 */
enum CommandType {
  STOP = 0,        ///< Stop command.
  START,           ///< Start command.
  RESET_DISTANCE,  ///< Reset distance measurement command.
  SET_SPEED,       ///< Set the speed to the value specified by the attribute.
  SWITCH_MODE,     ///< Switch to mode specified in the attribute.
};

/**
 * @enum ControlMode
 * @brief Defined the possible control modes the buggy can switch to
 */

enum ControlMode {
  LINE_FOLLOWING,   ///< Buggy follows line using PID speed control
  CRUISE_CONTROL,   ///< Buggy follows line using distance PID for speed control. 
  REMOTE_CONTROL    ///< Full remote control of the buggy.
};

/**
 * @struct CommandPacket
 * @brief Represents a command packet for controlling the system.
 */
struct CommandPacket {
  CommandType type;   ///< Type of command.
  uint32_t data;      ///< Additional command attribute.
};

/*
 * CONTROL PACKET
 */

struct ControlPacket {
  float x;
  float y;
};

/**
 * @union PacketContent
 * @brief Union containing different packet types.
 */
union PacketContent {
  StatusPacket statusPacket;    ///< Status packet data.
  CommandPacket commandPacket;  ///< Command packet data.
  ControlPacket controlPacket;
  LogPacket logPacket;          ///< Log packet data.
};


/**
 * @struct Packet
 * @brief Represents a generic packet containing different types of information.
 */
struct Packet {
  PacketType type;        ///< Type of the packet.
  PacketContent content;  ///< Packet content.
};

//const size_t MAX_PACKET_LENGTH = sizeof(Packet);

#endif //PACKET_TYPES_HPP