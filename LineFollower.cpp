#include "LineFollower.hpp"
#include "LeanStreamIO.hpp"

LineFollower::LineFollower(MotorDriver& leftMotor, MotorDriver& rightMotor, IrSensor& leftSensor, IrSensor& rightSensor, const PIDConstants& constants)
  : leftMotor(leftMotor), rightMotor(rightMotor), leftSensor(leftSensor), rightSensor(rightSensor), controller(constants) {
}

void LineFollower::update() {
  float error = this->leftSensor.getManualReading() - this->rightSensor.getManualReading();
  float per_err = (abs(error) / 1023);

  if (per_err < 0.20) per_err = 0;

  if (per_err > 0.50) per_err = 1.0; 

  int set_speed = this->target_speed * (1 - 1.1 * per_err);
  set_speed = constrain(set_speed, 0, 1000);

  /*if (this->leftSensor.getManualReading() > 800)
  {
    this->leftMotor.pwmOverride(0);
  } else if (this->rightSensor.getManualReading() > 800) {
    this->rightMotor.pwmOverride(0);
  } else*/

  if (error < 0) {
    //this->leftMotor.pwmOverride(round(set_speed));
    this->leftMotor.setSpeed(set_speed);
  } else {
    //this->rightMotor.pwmOverride(round(set_speed*0.80));
    this->rightMotor.setSpeed(set_speed);
  }
}
