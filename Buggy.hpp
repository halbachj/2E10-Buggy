#ifndef BUGGY_HPP
#define BUGGY_HPP

#include "MotorDriver.hpp"
#include "BuggyWifi.hpp"
#include "BuggyTCP.hpp"
#include "IrSensor.hpp"
#include "PinSetup.hpp"
#include "LineFollower.hpp"
#include "BuggyState.hpp"
#include "UltrasonicSensor.hpp"
#include "PacketFactory.hpp"
#include "LeanStreamIO.hpp"

class BuggyState;

/*
 * Buggy Class
 * 
 * The Buggy class contains all relevant peripherals mounted on the buggy, as well as some helper classes.
 * Buggy State is handled through a state machine which handles the different updates. Look into state machine class for further information.
 * State change is handled whenever a state change is reuqested by the state machine and the neccessarry calls are made.
 * The different Interrupt service routines (ISR) distribute to the underlying components that are associated with those e.g. motor encoders
 */

class Buggy {
private:
  MotorDriver& leftMotor;
  MotorDriver& rightMotor;

  IrSensor& leftIrSensor;
  IrSensor& rightIrSensor;

  UltrasonicSensor& ultrasonicSensor;

  BuggyWiFi& wifi;
  TcpServer& server;

  LineFollower& lineFollower;

  BuggyState* currentState;

  bool objectDetected = false;
  
public:

  Buggy(MotorDriver& leftMotor, MotorDriver& rightMotor, IrSensor& leftIrSensor, IrSensor& rightIrSensor,
   UltrasonicSensor& ultrasonicSensor, BuggyWiFi& wifi, TcpServer& server, LineFollower& lineFollower);
  // cannot copy
  Buggy(const Buggy& other) = delete;
  Buggy& operator=(const Buggy& other) = delete; 
  
  void update(unsigned int dt);

  void setState(BuggyState& newState);

  void handlePacket(Packet packet);
  void handleCommand(CommandPacket command);
  void sendStatusPacket();

  friend class TcpServer;
  friend class BuggyState;
  friend class IdleState;
  friend class CalibrationState;
  friend class LineFollowingState;
  
};


#endif // BUGGY_HPP
