#ifndef LINE_FOLLOWER_HPP
#define LINE_FOLLOWER_HPP

#include "MotorDriver.hpp"
#include "IrSensor.hpp"
#include "PIDController.hpp"

/**
 * @class LineFollower
 * @brief The line follower class is a helper class that allows for easy control of the buggy.
 * @details It holds both motors ans sensors and the PID controller for easy access.
 * On update the line follower class will poll the IR sensors calculate the error and turn based
 * on the calculated error. It also has PID capabilites for future PID based turning.
 */
class LineFollower {
public:
  /**
   * @brief TurnDirection is the direction the line follower is supposed to turn at at the next intersection.
   */
  enum class TurnDirection {
    LEFT,       ///< Turn left at next intersection.
    RIGHT,      ///< Turn right at next intersection.
    STRAIGHT    ///< Go straight ahead (default).
  };
  
private:
  IrSensor& leftSensor;
  IrSensor& rightSensor;

  MotorDriver& leftMotor;
  MotorDriver& rightMotor;

  PIDController controller; // Initialize the PID with the right coefficients

  int target_speed = 250; // 160
  int turning_threshold = 135;
  int turning_speed = 125;

  TurnDirection direction = TurnDirection::STRAIGHT;

public:
  /**
   * @brief Construct the line Follower
   * 
   * @param leftMoror the left motor driver
   * @param rightMotor the right motor driver
   * @param leftIrSensot the left IrSensor
   * @param leftIrSensot the right IrSensor
   * @param constants PID constants for the PID based turning
   */
  LineFollower(MotorDriver& leftMotor, MotorDriver& rightMotor, IrSensor& leftSensor, IrSensor& rightSensor, const PIDConstants& constants);
  /**
   * @brief Set the target speed of the line follower
   *
   * @param value the target speed to reach
   */
  void setSpeed(int value);
  /**
   * @brief Will read and adapt the motors each call. Needs to be called inside of loop to be useful.
   */ 
  void update(double dt);
  
  void setTurn(TurnDirection direction);

};


#endif // LINE_FOLLOWER_HPP
