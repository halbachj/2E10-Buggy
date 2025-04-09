#include "LineFollower.hpp"
#include "EmbeddedLogger.hpp"

LineFollower::LineFollower(MotorDriver& leftMotor, MotorDriver& rightMotor, IrSensor& leftSensor, IrSensor& rightSensor, const PIDConstants& constants)
  : leftMotor(leftMotor), rightMotor(rightMotor), leftSensor(leftSensor), rightSensor(rightSensor), controller(constants) {
}

void LineFollower::setSpeed(int value) {
  this->target_speed = value;
}

void LineFollower::update(double dt) {
  int leftReading, rightReading;
  leftReading = this->leftSensor.getManualReading();
  rightReading = this->rightSensor.getManualReading();

  switch (this->direction) {
    case TurnDirection::LEFT:
      leftReading = 0;
      this->turning_speed = 170;
      this->turning_threshold = 10;
      break;
    case TurnDirection::RIGHT:
      rightReading = 1024;
      this->turning_speed = 170;
      break;
    case TurnDirection::STRAIGHT:   // just do whatever we did before
    default:
      this->turning_speed = 125;
      this->turning_threshold = 135;
      break;
  }

  float err = (leftReading - rightReading);

  if (abs(err) <= this->turning_threshold) {   // pretty much on the line. Do nothing
    this->leftMotor.setSpeed(this->target_speed);
    this->rightMotor.setSpeed(this->target_speed);
    return;
  }

  if (err > this->turning_threshold) {
    this->leftMotor.setSpeed(this->target_speed+this->turning_speed);
    this->rightMotor.setSpeed(0);
  } else if (err < this->turning_threshold) {
    this->leftMotor.setSpeed(0);
    this->rightMotor.setSpeed(this->target_speed+this->turning_speed);
  }
}

void LineFollower::setTurn(TurnDirection direction) {
  this->direction = direction;
}
