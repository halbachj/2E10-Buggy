#include "LineFollower.hpp"
#include "LeanStreamIO.hpp"

LineFollower::LineFollower(MotorDriver& leftMotor, MotorDriver& rightMotor, IrSensor& leftSensor, IrSensor& rightSensor, const PIDConstants& constants)
  : leftMotor(leftMotor), rightMotor(rightMotor), leftSensor(leftSensor), rightSensor(rightSensor), controller(constants) {
}

void LineFollower::update() {
  float error = this->leftSensor.getManualReading() - this->rightSensor.getManualReading();

  int set_speed = this->target_speed * (1 - 1.0 * (abs(error) / 1023));
  if (error < 0) {
    this->leftMotor.pwmOverride(set_speed);
    //this->leftMotor.setSpeed(set_speed);
  } else {
    this->rightMotor.pwmOverride(set_speed);
    //this->rightMotor.setSpeed(set_speed);
  }
}
