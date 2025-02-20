#ifndef BUGGY_STATE_HPP
#define BUGGY_STATE_HPP

#include "Buggy.hpp"

class Buggy;  // Forward declaration

/**
 * @class BuggyState
 *
 * @brief The Buggy State class is an abstract class that represents the abstract state the buggy can be in.
 * From here all Buggy States are inherited e.g. IdleState or CalibrationState.
 * On state transition the different methods are called. First the exit method when the state is exited
 * and then the enter method when the state is entered. The update method is called on every update,
 * meaning as often as possible.
 **/

class BuggyState {
public:
    virtual void enter(Buggy& buggy) = 0;
    virtual void update(Buggy& buggy, double dt) = 0;
    virtual void exit(Buggy& buggy) = 0;
    virtual ~BuggyState() = default;
};

/*
 *   BUGGY STATES
 */

/**
 * @class IdleState
 * 
 * @brief The idle state represents a state in which the buggy operates normaly but is not driving. Can be compared to a stopped state.
 * In the idle state normal communication with the buggy is possible and the buggy can be resumed when prompted to start.
 * This state is reached whenever the buggy detects an obstacle and brakes, or when the buggy crashes by loosing the line or something similar.
 **/
class IdleState : public BuggyState {
public:
    static IdleState& instance() {
        static IdleState instance;
        return instance;
    }

    void enter(Buggy& buggy) override;
    void update(Buggy& buggy, double dt) override;
    void exit(Buggy& buggy) override;

private:
    IdleState() = default;  // Private constructor for singleton pattern
};


class ObjectDetectedState : public BuggyState {
private:
    int left_old_pwm = 0, right_old_pwm = 0;

public:

    static ObjectDetectedState& instance() {
        static ObjectDetectedState instance;
        return instance;
    }


    void enter(Buggy& buggy) override;
    void update(Buggy& buggy, double dt) override;
    void exit(Buggy& buggy) override;

private:
    ObjectDetectedState() = default;  // Private constructor for singleton pattern
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
  
  void enter(Buggy& buggy) override;
  void update(Buggy& buggy, double dt) override;
  void exit(Buggy& buggy) override;

private:
    CalibrationState() = default;  // Private constructor for singleton pattern
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

    void enter(Buggy& buggy) override;
    void update(Buggy& buggy, double dt) override;
    void exit(Buggy& buggy) override;

private:
    LineFollowingState() = default;  // Private constructor for singleton pattern
};


#endif //BUGGY_STATE
