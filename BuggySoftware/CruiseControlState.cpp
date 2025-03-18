#include "BuggyState.hpp"
#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;
using logLevel = EmbeddedLogger::LogLevel;

void CruiseControlState::enter(Buggy& buggy, BuggyState* oldState) {
  // Setup logic when entering idle state
}

void CruiseControlState::update(Buggy& buggy, double dt) {
  buggy.ultrasonicSensor.update();
  buggy.ultrasonicSensor.getReading();

  float speed = buggy.cruiseController.update(dt);
  buggy.lineFollower.setSpeed((int)abs(speed));
  buggy.lineFollower.update(dt);
  //buggy.leftMotor.setSpeed(speed);
  //buggy.rightMotor.setSpeed(speed);
  buggy.leftMotor.update(dt);
  buggy.rightMotor.update(dt);
}

void CruiseControlState::exit(Buggy& buggy, BuggyState* oldState) {}
