#include "PIDController.hpp"
#include "Arduino.h"
#include "Timer1.hpp"

PIDController::PIDController(const PIDConstants& constants): constants(constants) {
  this->prevTime = BuggyTimer1::counter;
}

float PIDController::update(float error, float dt) {
  float p, i, d;
  unsigned long currTime = BuggyTimer1::counter; 
  
  //float dt = (currTime - this->prevTime) / 2000.0; // Convert 1/2ms to s

  if (dt <= 0) dt = 1e-3; // Prevent division by zero, assume minimal dt

  this->accumulated_error += error * dt;
  this->accumulated_error = constrain(this->accumulated_error, -integralMax, integralMax); // constrain integral to bounds

  p = this->constants.Kp * error;
  i = this->constants.Ki * this->accumulated_error;
  d = this->constants.Kd * ((error - this->lastError) / dt);

  this->lastError = error;
  this->prevTime = currTime;

  return p + i + d;
}
