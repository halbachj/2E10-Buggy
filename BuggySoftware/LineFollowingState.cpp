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
<<<<<<< HEAD
    buggy.setState(ObjectDetectedState::instance());
=======
    buggy.setState(ObjectDetectedHandlerState::instance());
>>>>>>> c3ac23c (fixed the handler state for object detected and added an experimental udp server)
  }
}

void LineFollowingState::exit(Buggy& buggy, BuggyState* oldState) {
<<<<<<< HEAD
  // Cleanup logic before leaving idle state
=======
>>>>>>> c3ac23c (fixed the handler state for object detected and added an experimental udp server)
}
