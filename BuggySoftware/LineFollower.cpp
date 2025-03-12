#include "LineFollower.hpp"
#include "EmbeddedLogger.hpp"

LineFollower::LineFollower(MotorDriver& leftMotor, MotorDriver& rightMotor, IrSensor& leftSensor, IrSensor& rightSensor, const PIDConstants& constants)
  : leftMotor(leftMotor), rightMotor(rightMotor), leftSensor(leftSensor), rightSensor(rightSensor), controller(constants) {
}

void LineFollower::setSpeed(int value) {
  this->target_speed = value;
}

void LineFollower::update() {
  int leftReading, rightReading;
  leftReading = this->leftSensor.getManualReading();
  rightReading = this->rightSensor.getManualReading();

  int err = leftReading - rightReading;

  if (abs(err) <= 200) {   // pretty much on the line. Do nothing
    this->leftMotor.pwmOverride(this->target_speed);
    this->rightMotor.pwmOverride(this->target_speed);
    return;
  }

  if (err > 200) {
    this->leftMotor.pwmOverride(this->target_speed);
    this->rightMotor.pwmOverride(0);
  } else if (err < 200){
    this->leftMotor.pwmOverride(0);
    this->rightMotor.pwmOverride(this->target_speed);
  }
}
