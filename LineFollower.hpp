#ifndef LINE_FOLLOWER_HPP
#define LINE_FOLLOWER_HPP

#include "MotorDriver.hpp"
#include "IrSensor.hpp"
#include "PIDController.hpp"


/*
 * LINE FOLLOWER CLASS
 * The line follower class is a helper class that allows for easy control of the buggy.
 * It holds both motors ans sensors and the PID controller for easy access.
 * On update the line follower class will poll the IR sensors calculate the error,
 * then update the PID controller and use the correction from the PID controller
 * to change the motor speeds.
 */
class LineFollower {
private:
  IrSensor& leftSensor;
  IrSensor& rightSensor;

  MotorDriver& leftMotor;
  MotorDriver& rightMotor;

  PIDController controller; // Initialize the PID with the right coefficients

  int target_speed = 130; // 130
  
public:

  LineFollower(MotorDriver& leftMotor, MotorDriver& rightMotor, IrSensor& leftSensor, IrSensor& rightSensor, const PIDConstants& constants);
  void update();
  
};


#endif // LINE_FOLLOWER_HPP
