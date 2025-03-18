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

  //logger << logLevel::DEBUG << "[PID] dt: " << dt << EmbeddedLogger::endl;
  //logger << logLevel::DEBUG << "[PID] error: " << error << EmbeddedLogger::endl;
  //unsigned long currTime = BuggyTimer1::counter; 
  
  //float dt = (currTime - this->prevTime) / 2000.0; // Convert 1/2ms to s

  if (dt <= 0) dt = 1e-3; // Prevent division by zero, assume minimal dt

  this->accumulated_error += error * dt;
  this->accumulated_error = constrain(this->accumulated_error, -integralMax, integralMax); // constrain integral to bounds
  //logger << logLevel::DEBUG << "[PID] ACCUMULAGTED ERROR: " << this->accumulated_error << EmbeddedLogger::endl;

  p = this->constants.Kp * error;
  i = this->constants.Ki * this->accumulated_error;
  d = this->constants.Kd * ((error - this->lastError) / dt);

  /*logger << logLevel::DEBUG << "[PID] Kp: " << constants.Kp << EmbeddedLogger::endl;
  logger << logLevel::DEBUG << "[PID] Ki: " << constants.Ki << EmbeddedLogger::endl;
  logger << logLevel::DEBUG << "[PID] Kd: " << constants.Kd << EmbeddedLogger::endl;

  logger << logLevel::DEBUG << "[PID] P: " << p << EmbeddedLogger::endl;
  logger << logLevel::DEBUG << "[PID] I: " << i << EmbeddedLogger::endl;
  logger << logLevel::DEBUG << "[PID] D: " << d << EmbeddedLogger::endl;*/

  this->lastError = error;
  //this->prevTime = currTime;

  return p + i + d;
}
