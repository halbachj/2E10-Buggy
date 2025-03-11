#include "BuggyState.hpp"
#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;

void LineFollowingState::enter(Buggy& buggy, BuggyState* oldState) {
  // Setup logic when entering idle state
  buggy.ledMatrix.setMode(LINE_FOLLOWING);
}

void LineFollowingState::update(Buggy& buggy, double dt) {
  buggy.lineFollower.update();
  buggy.ultrasonicSensor.update();
  buggy.leftMotor.update(dt);
  buggy.rightMotor.update(dt);

  buggy.objectDetected = buggy.ultrasonicSensor.objectDetected();
  if (buggy.objectDetected) {
    buggy.setState(ObjectDetectedHandlerState::instance());
  }
}

void LineFollowingState::exit(Buggy& buggy, BuggyState* oldState) {
}
