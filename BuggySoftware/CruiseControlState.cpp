#include "BuggyState.hpp"
#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;

void CruiseControlState::enter(Buggy& buggy, BuggyState* oldState) {
  // Setup logic when entering idle state
}

void CruiseControlState::update(Buggy& buggy, double dt) {
  buggy.ultrasonicSensor.update();
  buggy.ultrasonicSensor.getReading();

  buggy.cruiseController.update(dt);

  buggy.leftMotor.update(dt);
  buggy.rightMotor.update(dt);
}

void CruiseControlState::exit(Buggy& buggy, BuggyState* oldState) {}
