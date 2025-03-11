#include "BuggyState.hpp"
#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;


void ObjectDetectedState::enter(Buggy& buggy, BuggyState* oldState) {
  // Setup logic when entering idle state
  this->left_old_pwm = buggy.leftMotor.getSpeed();
  this->right_old_pwm = buggy.rightMotor.getSpeed();
  buggy.leftMotor.pwmOverride(0);
  buggy.rightMotor.pwmOverride(0);
  buggy.ledMatrix.setMode(OBSTACLE_DETECTED);
}

void ObjectDetectedState::update(Buggy& buggy, double dt) {
  buggy.ultrasonicSensor.update();
  float distance = buggy.ultrasonicSensor.getReading();
  //logger << distance << EmbeddedLogger::endl;
  buggy.objectDetected = buggy.ultrasonicSensor.objectDetected();
  if (!buggy.objectDetected) { 
    buggy.setState(LineFollowingState::instance());
  }
}

void ObjectDetectedState::exit(Buggy& buggy, BuggyState* oldState) {
  // Cleanup logic before leaving idle state
}


/*
 * OBJECT DETECTED STATE HANDLER
 */

void ObjectDetectedHandlerState::enter(Buggy& buggy, BuggyState* oldState) {
  this->oldState = oldState;
}

void ObjectDetectedHandlerState::update(Buggy& buggy, double dt) {
  buggy.setState(ObjectDetectedState::instance());
}

void ObjectDetectedHandlerState::exit(Buggy& buggy, BuggyState* oldState) {
  buggy.setState(*this->oldState);
}
