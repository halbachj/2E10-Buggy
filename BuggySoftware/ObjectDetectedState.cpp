#include "BuggyState.hpp"
#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;
<<<<<<< HEAD


void ObjectDetectedState::enter(Buggy& buggy, BuggyState* oldState) {
  // Setup logic when entering idle state
=======
using logLevel = EmbeddedLogger::LogLevel;


void ObjectDetectedState::enter(Buggy& buggy, BuggyState* oldState) {
>>>>>>> c3ac23c (fixed the handler state for object detected and added an experimental udp server)
  this->left_old_pwm = buggy.leftMotor.getSpeed();
  this->right_old_pwm = buggy.rightMotor.getSpeed();
  buggy.leftMotor.pwmOverride(0);
  buggy.rightMotor.pwmOverride(0);
  buggy.ledMatrix.setMode(OBSTACLE_DETECTED);
}

void ObjectDetectedState::update(Buggy& buggy, double dt) {
  buggy.ultrasonicSensor.update();
  float distance = buggy.ultrasonicSensor.getReading();
<<<<<<< HEAD
  //logger << distance << EmbeddedLogger::endl;
=======
  logger << logLevel::INFO << "Object Distance: " << distance << EmbeddedLogger::endl;
>>>>>>> c3ac23c (fixed the handler state for object detected and added an experimental udp server)
  buggy.objectDetected = buggy.ultrasonicSensor.objectDetected();
  if (!buggy.objectDetected) { 
    buggy.setState(LineFollowingState::instance());
  }
}

<<<<<<< HEAD
void ObjectDetectedState::exit(Buggy& buggy, BuggyState* oldState) {
  // Cleanup logic before leaving idle state
}
=======
void ObjectDetectedState::exit(Buggy& buggy, BuggyState* oldState) {}
>>>>>>> c3ac23c (fixed the handler state for object detected and added an experimental udp server)


/*
 * OBJECT DETECTED STATE HANDLER
 */

void ObjectDetectedHandlerState::enter(Buggy& buggy, BuggyState* oldState) {
<<<<<<< HEAD
  this->oldState = oldState;
}

void ObjectDetectedHandlerState::update(Buggy& buggy, double dt) {
  buggy.setState(ObjectDetectedState::instance());
}

void ObjectDetectedHandlerState::exit(Buggy& buggy, BuggyState* oldState) {
  buggy.setState(*this->oldState);
=======
  this->savedState = oldState;
  if (oldState != &ObjectDetectedState::instance()) { // entering from different state / going to stop. Can compare because state is singleton
    buggy.setState(ObjectDetectedState::instance());
  }
}

void ObjectDetectedHandlerState::update(Buggy& buggy, double dt) {}

void ObjectDetectedHandlerState::exit(Buggy& buggy, BuggyState* oldState) {
  if (oldState == &ObjectDetectedState::instance()) {
    buggy.setState(*this->savedState);
  }
>>>>>>> c3ac23c (fixed the handler state for object detected and added an experimental udp server)
}
