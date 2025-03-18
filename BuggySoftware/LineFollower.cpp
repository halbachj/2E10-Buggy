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

  //float err = (leftReading - rightReading)/1023.0;
  float err = (leftReading - rightReading);
/*  logger << logLevel::DEBUG << "Line follower err " << err << EmbeddedLogger::endl;

  //float correction = this->controller.update(err, dt);

  //positive is a speed up in right motor
  int left_speed = this->target_speed * max(0, err);
  int right_speed = this->target_speed*(-1)*min(0, err);
  logger << "SPEED LEFT / RIGHT " << left_speed << " - " << right_speed << EmbeddedLogger::endl; 
  this->rightMotor.setSpeed(left_speed);
  this->leftMotor.setSpeed(right_speed);
*/

  if (abs(err) <= this->turning_threshold) {   // pretty much on the line. Do nothing
    //this->leftMotor.pwmOverride(this->target_speed);
    //this->rightMotor.pwmOverride(this->target_speed);
    this->leftMotor.setSpeed(this->target_speed);
    this->rightMotor.setSpeed(this->target_speed);
    return;
  }

  if (err > this->turning_threshold) {
    this->leftMotor.setSpeed(this->target_speed+150);
    this->rightMotor.setSpeed(0);
  } else if (err < this->turning_threshold){
    this->leftMotor.setSpeed(0);
    this->rightMotor.setSpeed(this->target_speed+150);
  }
}
