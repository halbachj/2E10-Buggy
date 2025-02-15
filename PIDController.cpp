#include "PIDController.hpp"
#include "Arduino.h"

PIDController::PIDController(const PIDConstants& constants): constants(constants) {
  this->prevTime = millis();
}

float PIDController::update(float error) {
  float p, i, d;
  unsigned long currTime = millis(); 
  float dt = (currTime - this->prevTime) / 1000.0; // Convert ms to s

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
