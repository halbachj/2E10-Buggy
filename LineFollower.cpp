#include "LineFollower.hpp"
#include "LeanStreamIO.hpp"

LineFollower::LineFollower(MotorDriver& leftMotor, MotorDriver& rightMotor, IrSensor& leftSensor, IrSensor& rightSensor, const PIDConstants& constants)
  : leftMotor(leftMotor), rightMotor(rightMotor), leftSensor(leftSensor), rightSensor(rightSensor), controller(constants) {
}

void LineFollower::update() {
  int leftReading, rightReading;
  leftReading = this->leftSensor.getManualReading();
  rightReading = this->rightSensor.getManualReading();

  int err = leftReading - RightReading;

  if (abs(err) <= 200) {   // pretty much on the line. Do nothing
    return;
  }

  if (err > 200) {
    this->leftMotor.pwmOverride(set_speed);
    this->rightMotor.pwmOverride(0);
  } else if (err < 200){
    this->leftMotor.pwmOverride(0);
    this->rightMotor.pwmOverride(set_speed);
  }
}
