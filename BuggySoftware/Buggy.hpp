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
#include "EmbeddedLogger.hpp"
#include "Matrix.hpp"
#include "CruiseControl.hpp"
#include "Camera.hpp"
#include "RoadSigns.hpp"

// Forward declerations
class BuggyState;
enum class BuggyStates;
class RoadSignRecognition;

/**
 * @class Buggy Class
 * 
 * @brief The Buggy class contains all relevant peripherals mounted on the buggy, as well as some helper classes.
 *
 * @details Buggy State is handled through a state machine which handles the different updates.
 * Look into state machine class for further information. State change is
 * handled whenever a state change is reuqested by the state machine and the neccessarry calls
 * are made.
 **/

class Buggy {
private:
  MotorDriver& leftMotor;
  MotorDriver& rightMotor;

  IrSensor& leftIrSensor;
  IrSensor& rightIrSensor;

  UltrasonicSensor& ultrasonicSensor;

  Camera& camera;

  Matrix ledMatrix;
  BuggyWiFi& wifi;
  TcpServer& server;

  LineFollower& lineFollower;
  CruiseControl& cruiseController;
  RoadSignRecognition& signRecognition;
   

  BuggyState* currentState;

  unsigned long last_status_sent = millis();
  unsigned int status_send_interval = 300;

  bool objectDetected = false;
  
  void switchControlMode(ControlMode mode);

public:

  /**
  * @brief Constructor for the buggy. Initialized with all neccessary components.
  *
  * @param leftMotor takes reference to left MotorDriver
  * @param rightMotor takes reference to right MotorDriver
  * @param leftIrSensor takes reference to left IrSensor Sensor
  * @param rightIrSensor takes referemce to right IrSensor Sensor 
  * @param ultrasonicSensor takes reference to the UltrasonicSensor
  * @param camera The HUSKYLENS Camera
  * @param wifi takes reference to the initialized BuggyWiFi to control the access point
  * @param server takes a reference to the running BuggyTCP server that handles ground station communication
  * @param lineFollower takes a reference to the LineFollower. This already contains reference to its neccessarry components
  * @param cruiseController takes a referenct to the CruiseController.  This already contains a reference to its neccessarry components
  **/

  Buggy(MotorDriver& leftMotor, MotorDriver& rightMotor, IrSensor& leftIrSensor, IrSensor& rightIrSensor,
   UltrasonicSensor& ultrasonicSensor, Camera& camera, BuggyWiFi& wifi, TcpServer& server, LineFollower& lineFollower, CruiseControl& cruiseController, RoadSignRecognition& signRecognition);
  // cannot copy
  Buggy(const Buggy& other) = delete;
  Buggy& operator=(const Buggy& other) = delete; 
 
  /**
  * @brief update is called during every loop. This calls the state machine update to update whatever state the buggy is in
  * 
  * @param dt is the time loop duration of the last loop in s
  **/
  void update(double dt);

  /**
  * @brief Changes the state of the buggy. This will call the necessary exit and enter methods of the state @see BuggyState.hpp
  * @details It will pass along the old states to those enter and exit methods so that the States can keep track of which state was entered before.
  * as an example @see ObjectDetectedHandlerState.
  *
  * @param newState reference to the state to change to
  **/
  void setState(BuggyState& newState);

  /**
  * @brief Gets the current state to be compared with @see BuggyStates 
  * 
  * @return the state
  **/
  BuggyStates getState();

  /**
  * @brief handles an incoming packet in the context of the buggy.
  *
  * @param packet the actual packet
  **/
  void handlePacket(Packet packet);
  /**
  * @brief decodes the CommandPacket and applies the command to the buggy.
  *
  * @param command The CommandPacket to be handled
  **/
  void handleCommand(CommandPacket command);
  /**
   * @brief handles the control packet for remote controlling the buggy
   *
   * @param control the control packet with the control inputs
   */
  void handleControlPacket(ControlPacket control);
  /**
  * @brief Sends a status packet to the ground station.
  **/
  void sendStatusPacket();
  /**
   * @brief updates the buggy vision
   */
  void updateRecognition();
  /**
   * @brief Sets the general speed of the buggy
   * @param speed the speed to run at 
   */
  void setSpeed(int speed);


  friend class TcpServer;
  friend class SignRecognition;
  friend class BuggyState;
  friend class IdleState;
  friend class ObjectDetectedState;
  friend class CalibrationState;
  friend class LineFollowingState;
  friend class LineFollowingState_TURN_LEFT;
  friend class LineFollowingState_TURN_RIGHT;
  friend class CruiseControl;
  friend class JustDriveState;
  friend class ObjectDetectedHandlerState;
  friend class CruiseControlState;
};


#endif // BUGGY_HPP
