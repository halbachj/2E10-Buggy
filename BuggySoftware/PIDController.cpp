#include "PIDController.hpp"
#include "Arduino.h"
#include "Timer1.hpp"

#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;
using logLevel = EmbeddedLogger::LogLevel;

PIDController::PIDController(const PIDConstants& constants): constants(constants) {
  //this->prevTime = BuggyTimer1::counter;
}

float PIDController::update(float error, double dt) {
  float p, i, d;

  if (dt <= 0) dt = 1e-3; // Prevent division by zero, assume minimal dt

  this->accumulated_error += error * dt;
  this->accumulated_error = constrain(this->accumulated_error, -integralMax, integralMax); // constrain integral to bounds

  p = this->constants.Kp * error;
  i = this->constants.Ki * this->accumulated_error;
  d = this->constants.Kd * ((error - this->lastError) / dt);

  this->lastError = error;

  return p + i + d;
}
