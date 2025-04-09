#include "BuggyState.hpp"

const char* getStateName(BuggyStates state) {
  switch (state) {
    case BuggyStates::IDLE:
      return " # IDLE State # ";
    case BuggyStates::JUST_DRIVING:
      return " # JUST_DRIVING State # ";
    case BuggyStates::CALIBRATION:
      return " # CALIBRATION State # ";
    case BuggyStates::CRUISE_CONTROL:
      return " # CRUISE_CONTROL State # ";
    case BuggyStates::LINE_FOLLOWING:
      return " # LINE_FOLLOWING State # ";
    case BuggyStates::LINE_FOLLOWING_LEFT:
      return " # LINE_FOLLOWING_LEFT State # ";    
    case BuggyStates::LINE_FOLLOWING_RIGHT:
      return " # LINE_FOLLOWING_RIGHT State # ";      
    case BuggyStates::OBJECT_DETECTED:
      return " # OBJECT_DETECTED State # ";
    case BuggyStates::OBJECT_DETECTED_HANDLER:
      return " # OBJECT_DTECTED_HANDLER State # ";
    default:
      return " # UNKNOWN STATE # ";
  }
}