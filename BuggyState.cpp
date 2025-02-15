#include "BuggyState.hpp"
#include "Logger.hpp"


/*
 * IDLE STATE LOGIC
 */

void IdleState::enter(Buggy& buggy) {
  // Setup logic when entering idle state
  buggy.leftMotor.pwmOverride(0);
  buggy.rightMotor.pwmOverride(0);
}

void IdleState::update(Buggy& buggy, unsigned int dt) {
  buggy.ultrasonicSensor.update();
  float distance = buggy.ultrasonicSensor.getReading();
  //mcu::logger << String(distance).c_str() << mcu::LeanStreamIO::endl;
  if (distance > 20) {
    buggy.setState(LineFollowingState::instance());
  }
}

void IdleState::exit(Buggy& buggy) {
  // Cleanup logic before leaving idle state
}


/*
 * CALIBRATION STATE LOGIC
 */

void CalibrationState::enter(Buggy& buggy) {
  // Start buggy and rotate around itself
}

void CalibrationState::update(Buggy& buggy, unsigned int dt) {
  
}

void CalibrationState::exit(Buggy& buggy) {
  // Set IrSensor min and max values
  //ybuggy.leftSensor.setCalibrationRange()
}


/*
 * LINE FOLLOWING STATE LOGIC
 */

void LineFollowingState::enter(Buggy& buggy) {
  // Setup logic when entering idle state
}

void LineFollowingState::update(Buggy& buggy, unsigned int dt) {
  buggy.lineFollower.update();
  buggy.ultrasonicSensor.update();

  float distance = buggy.ultrasonicSensor.getReading();
  if (distance < 20) {
    buggy.setState(IdleState::instance());
  }

  //buggy.leftMotor.update(dt);
  //buggy.rightMotor.update(dt);
}

void LineFollowingState::exit(Buggy& buggy) {
  // Cleanup logic before leaving idle state
}
