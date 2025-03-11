#include "BuggyState.hpp"
#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;

void JustDriveState::enter(Buggy& buggy, BuggyState* oldState) {
  // Setup logic when entering idle state
}

void JustDriveState::update(Buggy& buggy, double dt) {
<<<<<<< HEAD
  //buggy.ultrasonicSensor.update();


  /*buggy.objectDetected = buggy.ultrasonicSensor.objectDetected();
  if (buggy.objectDetected) { 
    buggy.setState(ObjectDetectedState::instance());
  }*/
=======
  buggy.ultrasonicSensor.update();

  buggy.objectDetected = buggy.ultrasonicSensor.objectDetected();
  if (buggy.objectDetected) { 
    buggy.setState(ObjectDetectedHandlerState::instance());
  }

>>>>>>> c3ac23c (fixed the handler state for object detected and added an experimental udp server)
  buggy.leftMotor.update(dt);
  buggy.rightMotor.update(dt);
}

void JustDriveState::exit(Buggy& buggy, BuggyState* oldState) {
}
