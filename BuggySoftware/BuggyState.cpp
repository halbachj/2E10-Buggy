#include "BuggyState.hpp"
#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;

/*
 * IDLE STATE LOGIC
 */

void IdleState::enter(Buggy& buggy, BuggyState* oldState) {
  // Setup logic when entering idle state
  buggy.leftMotor.pwmOverride(0);
  buggy.rightMotor.pwmOverride(0);
  buggy.ledMatrix.setMode(STOPPED);
}

void IdleState::update(Buggy& buggy, double dt) {
  buggy.ultrasonicSensor.update();
}

void IdleState::exit(Buggy& buggy, BuggyState* oldState) {
  // Cleanup logic before leaving idle state
}

/*
 * OBJECT DETECTED
 */

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
    buggy.setState(ObjectDetectedHandlerState::instance());
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


/*
 * CALIBRATION STATE LOGIC
 */

void CalibrationState::enter(Buggy& buggy, BuggyState* oldState) {
  this->leftMax = buggy.leftIrSensor.getManualReading();
  this->leftMin = this->leftMax;
  this->rightMax = buggy.leftIrSensor.getManualReading();
  this->rightMin = this->rightMax;

  buggy.leftMotor.backward();
  buggy.rightMotor.forward();
  buggy.leftMotor.setSpeed(600);
  buggy.rightMotor.setSpeed(600);
}

void CalibrationState::update(Buggy& buggy, double dt) {
  unsigned int leftReading, rightReading;
  leftReading = buggy.leftIrSensor.getManualReading();
  rightReading = buggy.rightIrSensor.getManualReading();

  if(this->leftMax < leftReading) this->leftMax = leftReading;
  if(this->leftMin > leftReading) this->leftMin = leftReading;

  if(this->rightMax < rightReading) this->rightMax = rightReading;
  if(this->rightMin > rightReading) this->rightMin = rightReading;


  // LEFT CROSS CHECK
  if(leftReading >= this->upperBoudary && !this->leftOnLine) { // Just got on the line
    this->leftOnLine = true;
  }
  if(leftReading <= this->lowerBundary && this->leftOnLine) { // Just got off the line
    this->leftOnLine = false;
    ++this->leftCrossed;
  }


  // RIGHT CROSS CHECK
  if(rightReading >= this->upperBoudary && !this->rightOnLine) { // Just got on the line
    this->rightOnLine = true;
  }
  if(rightReading <= this->lowerBundary && this->rightOnLine) { // Just got off the line
    this->rightOnLine = false;
    ++this->rightCrossed;
  }

  if (this->leftCrossed >=3 && this->leftCrossed - this->rightCrossed > 1) { // Crossed line twice and now in between
    buggy.setState(LineFollowingState::instance()); // Go to Line following
  }
}

void CalibrationState::exit(Buggy& buggy, BuggyState* oldState) {
  // Set IrSensor min and max values
  buggy.leftMotor.forward();
  buggy.rightMotor.forward();
  buggy.leftMotor.setSpeed(0);
  buggy.rightMotor.setSpeed(0);
  buggy.leftIrSensor.setCalibrationRange(this->leftMin, this->leftMax);
  buggy.rightIrSensor.setCalibrationRange(this->rightMin, this->rightMax);
}


/*
 * LINE FOLLOWING STATE LOGIC
 */

void LineFollowingState::enter(Buggy& buggy, BuggyState* oldState) {
  // Setup logic when entering idle state
  buggy.ledMatrix.setMode(LINE_FOLLOWING);
}

void LineFollowingState::update(Buggy& buggy, double dt) {
  buggy.lineFollower.update();
  buggy.ultrasonicSensor.update();

  //buggy.leftMotor.update(dt);
  //buggy.rightMotor.update(dt);

  buggy.objectDetected = buggy.ultrasonicSensor.objectDetected();
  if (buggy.objectDetected) {
    buggy.setState(ObjectDetectedHandlerState::instance());
  }
}

void LineFollowingState::exit(Buggy& buggy, BuggyState* oldState) {
  // Cleanup logic before leaving idle state
}
