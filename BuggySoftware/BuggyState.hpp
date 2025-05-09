#ifndef BUGGY_STATE_HPP
#define BUGGY_STATE_HPP

#include "Buggy.hpp"

class Buggy;  // Forward declaration

enum class BuggyStates {
  IDLE,
  JUST_DRIVING,
  CALIBRATION,
  CRUISE_CONTROL,
  LINE_FOLLOWING,
  LINE_FOLLOWING_LEFT,
  LINE_FOLLOWING_RIGHT,
  OBJECT_DETECTED,
  OBJECT_DETECTED_HANDLER,
};


const char* getStateName(BuggyStates state);

/**
 * @class BuggyState
 *
 * @brief The Buggy State class is an abstract class that represents the abstract state the buggy can be in.
 * @details From here all Buggy States are inherited e.g. IdleState or CalibrationState.
 * On state transition the different methods are called. First the exit method when the state is exited
 * and then the enter method when the state is entered. The update method is called on every update,
 * meaning as often as possible.
 **/

class BuggyState {
private:
  BuggyStates state;
public:
    BuggyState(BuggyStates state) : state(state) {};
    virtual void enter(Buggy& buggy, BuggyState* oldState) = 0;
    virtual void update(Buggy& buggy, double dt) = 0;
    virtual void exit(Buggy& buggy, BuggyState* newState) = 0;
    virtual ~BuggyState() = default;
    BuggyStates getState() {return state;};
};

/*
 *   BUGGY STATES
 */

/**
 * @class IdleState
 * 
 * @brief The idle state represents a state in which the buggy operates normaly but is not driving.
 * @details Can be compared to a stopped state. In the idle state normal communication with the buggy
 * is possible and the buggy can be resumed when prompted to start. This state is reached whenever
 * the buggy detects an obstacle and brakes, or when the buggy crashes by loosing the line or something similar.
 **/
class IdleState : public BuggyState {
private:
public:
    static IdleState& instance() {
        static IdleState instance;
        return instance;
    }

    void enter(Buggy& buggy, BuggyState* oldState) override;
    void update(Buggy& buggy, double dt) override;
    void exit(Buggy& buggy, BuggyState* oldState) override;

private:
    IdleState() : BuggyState(BuggyStates::IDLE) {}  // Private constructor for singleton pattern
};


/**
 * @class ObjectDetectedState
 * 
 * @brief The state the Buggy enters when an object is detected during driving
 * @details Compared to the IdleState the buggy updates the ultrasonic sensor and 
 * resumes operation when the object is removed.
 **/

class ObjectDetectedState : public BuggyState {
private:
    int left_old_pwm = 0, right_old_pwm = 0;

public:

    static ObjectDetectedState& instance() {
        static ObjectDetectedState instance;
        return instance;
    }

    void enter(Buggy& buggy, BuggyState* oldState) override;
    void update(Buggy& buggy, double dt) override;
    void exit(Buggy& buggy, BuggyState* oldState) override;

private:
    ObjectDetectedState() : BuggyState(BuggyStates::OBJECT_DETECTED) {};  // Private constructor for singleton pattern
};


/**
 * @class ObjectDetectedHandlerState
 * 
 * @brief This state is entered before the buggy goes into ObjectDetectedState. It is used to save the previous state and return back to it.
 * @details Whenever this state is entered it will save the current state and enter ObjectDetectedState. When ObjectDetectedState is left,
 * this state is entered again and the old previous state is resumed. Almost like a stack based machine.
 **/
class ObjectDetectedHandlerState : public BuggyState {
private:
  BuggyState* savedState;
public:

    static ObjectDetectedHandlerState& instance() {
        static ObjectDetectedHandlerState instance;
        return instance;
    }


    void enter(Buggy& buggy, BuggyState* oldState) override;
    void update(Buggy& buggy, double dt) override;
    void exit(Buggy& buggy, BuggyState* oldState) override;

private:
    ObjectDetectedHandlerState() : BuggyState(BuggyStates::OBJECT_DETECTED_HANDLER) {};  // Private constructor for singleton pattern
};

/**
 * @class CalibrationState
 *
 * @brief The calibration state of the buggy is the first state it enters after being placed on the track. The buggy will perform a full rotation
 * an collect as many datapoints as possible from the IR sensors. On exit these calibration values are set in the corresponding IR sensors
 * as calibration data. The state is then changed to idle and the buggy waits for commands.
 **/
class CalibrationState : public BuggyState {
private:
  bool leftOnLine = false, rightOnLine = false;
  unsigned int leftMax = 0, leftMin = 0, rightMax = 0, rightMin = 0;
  unsigned short leftCrossed = 0, rightCrossed = 0;
  unsigned int upperBoudary = 768, lowerBundary = 255;

public:
  static CalibrationState& instance() {
      static CalibrationState instance;
      return instance;
  }
  
  void enter(Buggy& buggy, BuggyState* oldState) override;
  void update(Buggy& buggy, double dt) override;
  void exit(Buggy& buggy, BuggyState* oldState) override;

private:
    CalibrationState() : BuggyState(BuggyStates::CALIBRATION) {};  // Private constructor for singleton pattern
};

/**
 * @class LineFollowingState
 *
 * @brief In the Line following state the buggy is trying to follow the line using the IR sensors. The buggy also checks for obstacles 
 * in the way and brakes when it detects any. The buggy also reports neccessarry information back to the ground station through the 
 * TCP server.
 */

class LineFollowingState : public BuggyState {
public:
    static LineFollowingState& instance() {
        static LineFollowingState instance;
        return instance;
    }

    void enter(Buggy& buggy, BuggyState* oldState) override;
    void update(Buggy& buggy, double dt) override;
    void exit(Buggy& buggy, BuggyState* oldState) override;
  
private:
    LineFollowingState() : BuggyState(BuggyStates::LINE_FOLLOWING) {};  // Private constructor for singleton pattern
};


/**
 * @class LineFollowingState_TURN_RIGHT
 *
 * @brief A special sub state of LineFollowing which preferes a right turn when following the line.
 *
 * This is used to turn onto a different track at intersections.
 */

class LineFollowingState_TURN_RIGHT : public BuggyState {
private:
  unsigned long hitIntersectionTime;
  bool hitIntersection = false; // goes high when both sensors are on the intersection
public:
    static LineFollowingState_TURN_RIGHT& instance() {
      static LineFollowingState_TURN_RIGHT instance;
      return instance;
    }

    void enter(Buggy& buggy, BuggyState* oldState) override;
    void update(Buggy& buggy, double dt) override;
    void exit(Buggy& buggy, BuggyState* oldState) override;
  
private:
    LineFollowingState_TURN_RIGHT() : BuggyState(BuggyStates::LINE_FOLLOWING_RIGHT) {};;  // Private constructor for singleton pattern
};

/**
 * @class LineFollowingState_TURN_LEFT
 *
 * @brief A special sub state of LineFollowing which preferes a left turn when following the line.
 *
 * This is used to turn onto a different track at intersections.
 */

class LineFollowingState_TURN_LEFT : public BuggyState {
private:
  unsigned long hitIntersectionTime;
  bool hitIntersection = false; // goes high when both sensors are on the intersection
public:
    static LineFollowingState_TURN_LEFT& instance() {
        static LineFollowingState_TURN_LEFT instance;
        return instance;
    }

    void enter(Buggy& buggy, BuggyState* oldState) override;
    void update(Buggy& buggy, double dt) override;
    void exit(Buggy& buggy, BuggyState* oldState) override;
  
private:
    LineFollowingState_TURN_LEFT() : BuggyState(BuggyStates::LINE_FOLLOWING_LEFT) {};  // Private constructor for singleton pattern
};


/**
 * @class CruiseControl
 *
 * @brief Cruise control state keeps the distance to an obstacle infront of the buggy constant using the Ultrasonic sensor. The buggy continues 
 * to follow the line and adjusts its speed accordingly.
 */

class CruiseControlState : public BuggyState {
public:
    static CruiseControlState& instance() {
        static CruiseControlState instance;
        return instance;
    }

    void enter(Buggy& buggy, BuggyState* oldState) override;
    void update(Buggy& buggy, double dt) override;
    void exit(Buggy& buggy, BuggyState* oldState) override;
  
private:
    CruiseControlState() : BuggyState(BuggyStates::CRUISE_CONTROL) {};  // Private constructor for singleton pattern
};



class JustDriveState : public BuggyState {
public:
    static JustDriveState& instance() {
        static JustDriveState instance;
        return instance;
    }

    void enter(Buggy& buggy,  BuggyState* oldState) override;
    void update(Buggy& buggy, double dt) override;
    void exit(Buggy& buggy, BuggyState* oldState) override;

private:
    JustDriveState() : BuggyState(BuggyStates::JUST_DRIVING) {};  // Private constructor for singleton pattern
};

#endif //BUGGY_STATE
