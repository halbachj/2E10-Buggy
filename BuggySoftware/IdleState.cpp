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
  buggy.ledMatrix.setMode(BuggyMode::STOPPED);

}

void IdleState::update(Buggy& buggy, double dt) {
  buggy.ultrasonicSensor.update();
}

void IdleState::exit(Buggy& buggy, BuggyState* oldState) {
  // Cleanup logic before leaving idle state
}
