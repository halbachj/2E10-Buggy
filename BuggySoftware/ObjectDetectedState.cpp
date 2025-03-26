#include "BuggyState.hpp"
#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;
using logLevel = EmbeddedLogger::LogLevel;


void ObjectDetectedState::enter(Buggy& buggy, BuggyState* oldState) {
  buggy.leftMotor.setSpeed(0);
  buggy.rightMotor.setSpeed(0);
  buggy.ledMatrix.setMode(BuggyMode::OBSTACLE_DETECTED);
}

void ObjectDetectedState::update(Buggy& buggy, double dt) {
  buggy.ultrasonicSensor.update();
  float distance = buggy.ultrasonicSensor.getReading();
  logger << logLevel::INFO << F("Object Distance: ") << distance << EmbeddedLogger::endl;
  buggy.objectDetected = buggy.ultrasonicSensor.objectDetected();
  if (!buggy.objectDetected) { 
    buggy.setState(LineFollowingState::instance());
  }
  buggy.leftMotor.update(dt);
  buggy.rightMotor.update(dt);
}

void ObjectDetectedState::exit(Buggy& buggy, BuggyState* oldState) {
}


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
