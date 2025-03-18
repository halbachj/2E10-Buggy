#include "BuggyState.hpp"
#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;
using logLevel = EmbeddedLogger::LogLevel;


void ObjectDetectedState::enter(Buggy& buggy, BuggyState* oldState) {
  this->left_old_pwm = buggy.leftMotor.getSpeed();
  this->right_old_pwm = buggy.rightMotor.getSpeed();
  buggy.leftMotor.pwmOverride(0);
  buggy.rightMotor.pwmOverride(0);
  buggy.ledMatrix.setMode(BuggyMode::OBSTACLE_DETECTED);
}

void ObjectDetectedState::update(Buggy& buggy, double dt) {
  buggy.ultrasonicSensor.update();
  float distance = buggy.ultrasonicSensor.getReading();
  logger << logLevel::INFO << "Object Distance: " << distance << EmbeddedLogger::endl;
  buggy.objectDetected = buggy.ultrasonicSensor.objectDetected();
  if (!buggy.objectDetected) { 
    buggy.setState(LineFollowingState::instance());
  }
}

void ObjectDetectedState::exit(Buggy& buggy, BuggyState* oldState) {}


/*
 * OBJECT DETECTED STATE HANDLER
 */

void ObjectDetectedHandlerState::enter(Buggy& buggy, BuggyState* oldState) {
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
}
