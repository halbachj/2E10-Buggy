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
  buggy.updateRecognition();

  /*buggy.objectDetected = buggy.ultrasonicSensor.objectDetected();
  if (buggy.objectDetected) {
    buggy.setState(ObjectDetectedHandlerState::instance());
  }*/
}

void LineFollowingState::exit(Buggy& buggy, BuggyState* oldState) {
}


void LineFollowingState_TURN_RIGHT::enter(Buggy& buggy, BuggyState* oldState) {
  // Setup logic when entering idle state
  buggy.ledMatrix.setMode(BuggyMode::LINE_FOLLOWING);
}

void LineFollowingState_TURN_RIGHT::update(Buggy& buggy, double dt) {
    if(!this->hitIntersection) {
    buggy.lineFollower.update(dt);
    buggy.leftMotor.update(dt);
    buggy.rightMotor.update(dt);
  } else {
    buggy.leftMotor.pause(dt); // trick PID into not accumulating error
    buggy.rightMotor.pause(dt);
  }

  logger << logLevel::DEBUG << "IR VALUE LEFT: " << buggy.leftIrSensor.getManualReading() << " IS HIGH: " << buggy.leftIrSensor.isHigh() << EmbeddedLogger::endl;
  logger << logLevel::DEBUG << "IR VALUE RIGHT: " << buggy.rightIrSensor.getManualReading() << " IS HIGH: " << buggy.rightIrSensor.isHigh() << EmbeddedLogger::endl;

  if (!this->hitIntersection && buggy.leftIrSensor.isHigh() && buggy.rightIrSensor.isHigh()) {
    logger << logLevel::DEBUG << "HIT INTERSECTION TURNING RIGHT NOW" << EmbeddedLogger::endl;
    //buggy.setSpeed(50);
    buggy.leftMotor.pwmOverride(0);
    buggy.leftMotor.pwmOverride(0);
    buggy.lineFollower.setTurn(LineFollower::TurnDirection::LEFT);
    this->hitIntersection = true;
    this->hitIntersectionTime = millis();
    buggy.leftMotor.pwmOverride(180);
  }

  if (this->hitIntersection && !buggy.rightIrSensor.isHigh()) {//|| millis() - this->hitIntersectionTime >= 5000)) {
    logger << logLevel::DEBUG << "PASSED INTERSECTION GOING STRAIGHT NOW" << EmbeddedLogger::endl;
    buggy.setState(LineFollowingState::instance());
    buggy.rightMotor.pwmOverride(0);
    buggy.leftMotor.pwmOverride(0);
  }
}

void LineFollowingState_TURN_RIGHT::exit(Buggy& buggy, BuggyState* oldState) {
  buggy.lineFollower.setTurn(LineFollower::TurnDirection::STRAIGHT);
  buggy.signRecognition.clearSign();
  buggy.setSpeed(200);
}


void LineFollowingState_TURN_LEFT::enter(Buggy& buggy, BuggyState* oldState) {
  // Setup logic when entering idle state
  buggy.ledMatrix.setMode(BuggyMode::LINE_FOLLOWING);
  this->hitIntersection = false;
}

void LineFollowingState_TURN_LEFT::update(Buggy& buggy, double dt) {
  if(!this->hitIntersection) {
    buggy.lineFollower.update(dt);
    buggy.leftMotor.update(dt);
    buggy.rightMotor.update(dt);
  } else {
    buggy.leftMotor.pause(dt); // trick PID into not accumulating error
    buggy.rightMotor.pause(dt);
  }
  

  /*buggy.ultrasonicSensor.update();
  buggy.objectDetected = buggy.ultrasonicSensor.objectDetected();
  if (buggy.objectDetected) {
    buggy.setState(ObjectDetectedHandlerState::instance());
  }*/

  logger << logLevel::DEBUG << "IR VALUE LEFT: " << buggy.leftIrSensor.getManualReading() << " IS HIGH: " << buggy.leftIrSensor.isHigh() << EmbeddedLogger::endl;
  logger << logLevel::DEBUG << "IR VALUE RIGHT: " << buggy.rightIrSensor.getManualReading() << " IS HIGH: " << buggy.rightIrSensor.isHigh() << EmbeddedLogger::endl;

  if (!this->hitIntersection && buggy.leftIrSensor.isHigh() && buggy.rightIrSensor.isHigh()) {
    logger << logLevel::DEBUG << "HIT INTERSECTION TURNING LEFT NOW" << EmbeddedLogger::endl;
    //buggy.setSpeed(50);
    buggy.leftMotor.pwmOverride(0);
    buggy.leftMotor.pwmOverride(0);
    buggy.lineFollower.setTurn(LineFollower::TurnDirection::LEFT);
    this->hitIntersection = true;
    this->hitIntersectionTime = millis();
    buggy.rightMotor.pwmOverride(180);
  }

  if (this->hitIntersection && !buggy.leftIrSensor.isHigh()) {//|| millis() - this->hitIntersectionTime >= 5000)) {
    logger << logLevel::DEBUG << "PASSED INTERSECTION GOING STRAIGHT NOW" << EmbeddedLogger::endl;
    buggy.setState(LineFollowingState::instance());
    buggy.rightMotor.pwmOverride(0);
    buggy.leftMotor.pwmOverride(0);
  }

}

void LineFollowingState_TURN_LEFT::exit(Buggy& buggy, BuggyState* oldState) {
  buggy.lineFollower.setTurn(LineFollower::TurnDirection::STRAIGHT);
  buggy.signRecognition.clearSign();
  buggy.setSpeed(200);
}
