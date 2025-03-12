#include "CruiseControl.hpp"

CruiseControl::CruiseControl(UltrasonicSensor& ussensor, LineFollower& lineFollower, PIDConstants constansts) : ussensor(ussensor), lineFollower(lineFollower), controller(constansts) {}

void CruiseControl::update(float dt) {
  float distance, error, correction;

  distance = this->ussensor.getReading();
  error = distance - this->target_distance;
  correction = this->controller.update(error, dt);
  
  this->lineFollower.setSpeed(correction);
}
  
