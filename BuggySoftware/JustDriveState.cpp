#include "BuggyState.hpp"
#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;

void JustDriveState::enter(Buggy& buggy, BuggyState* oldState) {
  // Setup logic when entering idle state
}

void JustDriveState::update(Buggy& buggy, double dt) {
  buggy.ultrasonicSensor.update();

  buggy.objectDetected = buggy.ultrasonicSensor.objectDetected();
  if (buggy.objectDetected) { 
    buggy.setState(ObjectDetectedHandlerState::instance());
  }

  buggy.leftMotor.pause(dt);
  buggy.rightMotor.pause(dt);
}

void JustDriveState::exit(Buggy& buggy, BuggyState* oldState) {
}
