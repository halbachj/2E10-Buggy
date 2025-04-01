#include "BuggyState.hpp"
#include "EmbeddedLogger.hpp"
#include "LineFollower.hpp"

using EmbeddedLogger::logger;

void LineFollowingState::enter(Buggy& buggy, BuggyState* oldState) {
  // Setup logic when entering idle state
  buggy.ledMatrix.setMode(BuggyMode::LINE_FOLLOWING);
}

void LineFollowingState::update(Buggy& buggy, double dt) {
  buggy.lineFollower.update(dt);
  buggy.ultrasonicSensor.update();
  buggy.leftMotor.update(dt);
  buggy.rightMotor.update(dt);
  buggy.camera.update();
  buggy.signRecognition.update();

  buggy.objectDetected = buggy.ultrasonicSensor.objectDetected();
  if (buggy.objectDetected) {
    buggy.setState(ObjectDetectedHandlerState::instance());
  }
}

void LineFollowingState::exit(Buggy& buggy, BuggyState* oldState) {
}


void LineFollowingState_TURN_RIGHT::enter(Buggy& buggy, BuggyState* oldState) {
  // Setup logic when entering idle state
  buggy.ledMatrix.setMode(BuggyMode::LINE_FOLLOWING);
  buggy.lineFollower.setTurn(LineFollower::TurnDirection::RIGHT);
}

void LineFollowingState_TURN_RIGHT::update(Buggy& buggy, double dt) {
  buggy.lineFollower.update(dt);
  buggy.ultrasonicSensor.update();
  buggy.leftMotor.update(dt);
  buggy.rightMotor.update(dt);
  buggy.camera.update();
  buggy.signRecognition.update();

  buggy.objectDetected = buggy.ultrasonicSensor.objectDetected();
  if (buggy.objectDetected) {
    buggy.setState(ObjectDetectedHandlerState::instance());
  }

  if (buggy.leftIrSensor.isHigh() && buggy.rightIrSensor.isHigh()) {
    this->hitIntersection = true;
  }

  if (this->hitIntersection && buggy.leftIrSensor.isLow() && buggy.rightIrSensor.isLow()) {
    this->hitIntersection = false;
    buggy.setState(LineFollowingState::instance());
  }
}

void LineFollowingState_TURN_RIGHT::exit(Buggy& buggy, BuggyState* oldState) {
  buggy.lineFollower.setTurn(LineFollower::TurnDirection::STRAIGHT);
}


void LineFollowingState_TURN_LEFT::enter(Buggy& buggy, BuggyState* oldState) {
  // Setup logic when entering idle state
  buggy.ledMatrix.setMode(BuggyMode::LINE_FOLLOWING);
  buggy.lineFollower.setTurn(LineFollower::TurnDirection::LEFT);
}

void LineFollowingState_TURN_LEFT::update(Buggy& buggy, double dt) {
  buggy.lineFollower.update(dt);
  buggy.ultrasonicSensor.update();
  buggy.leftMotor.update(dt);
  buggy.rightMotor.update(dt);
  buggy.camera.update();
  buggy.signRecognition.update();

  buggy.objectDetected = buggy.ultrasonicSensor.objectDetected();
  if (buggy.objectDetected) {
    buggy.setState(ObjectDetectedHandlerState::instance());
  }

  if (buggy.leftIrSensor.isHigh() && buggy.rightIrSensor.isHigh()) {
    this->hitIntersection = true;
  }

  if (this->hitIntersection && buggy.leftIrSensor.isLow() && buggy.rightIrSensor.isLow()) {
    this->hitIntersection = false;
    buggy.setState(LineFollowingState::instance());
  }

}

void LineFollowingState_TURN_LEFT::exit(Buggy& buggy, BuggyState* oldState) {
  buggy.lineFollower.setTurn(LineFollower::TurnDirection::STRAIGHT);
}
