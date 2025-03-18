#include "CruiseControl.hpp"

#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;
using logLevel = EmbeddedLogger::LogLevel;

CruiseControl::CruiseControl(UltrasonicSensor& ussensor, LineFollower& lineFollower, const PIDConstants& constants) : ussensor(ussensor), lineFollower(lineFollower), controller(constants) {}

float CruiseControl::update(double dt) {
  float distance, error, correction;
  logger << logLevel::DEBUG << "CRUISE CONTROL DT " << dt << EmbeddedLogger::endl;

  distance = this->ussensor.getReading();
  logger << logLevel::DEBUG << "CRUISE CONTROL MEASURED DISTANCE " << distance << EmbeddedLogger::endl;

  error = distance - this->target_distance;
  logger << logLevel::DEBUG << "CRUISE CONTROL ERROR " << error << EmbeddedLogger::endl;

  /*logger << logLevel::DEBUG << "CRUISE CONTROL CONSTANTS:  ";
  logger << this->constants.Kp << " ";
  logger << this->constants.Ki << " ";
  logger << this->constants.Kd << " " << EmbeddedLogger::endl;*/

  correction = this->controller.update(error, dt);
  logger << logLevel::DEBUG << "CRUISE CONTROL CORRECTION " << correction << EmbeddedLogger::endl;
  
  correction = min(correction, this->max_speed);
  return max(0, correction);
  //this->lineFollower.setSpeed(abs(correction));
  //maybe updtae line follower here
}
  
