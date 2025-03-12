#ifndef CRUISE_CONTROL_HPP
#define CRUISE_CONTROL_HPP

#include "MotorDriver.hpp"
#include "IrSensor.hpp"
#include "PIDController.hpp"
#include "LineFollower.hpp"
#include "UltrasonicSensor.hpp"


/**
 * @brief The CruiseControl helper class allows holds helper functions to allow
 * the buggy to hold a constant distance to an obstacle in front.
 *
 * This is done using a cascade PID controller, as this PID controller controls
 * distance and the second PID controller controls the rotational speed of the wheels.
 */
class CruiseControl {
private:
  LineFollower& lineFollower;
  UlstrasonicSensor& ussensor;
  PIDController controller; // Initialize the PID with the right coefficients

  int target_distance = 20; // cm
  
public:
  /**
   * @brief Construct the Cruise controller
   *
   * @param lineFollower the line follower helper class
   */
  LineFollower(UltrasonicSensor& ussensor, LineFolower& lineFollower, PIDConstants constansts);
  /**
   * @brief Will read and adapt the line follower speed at each call. Needs to be called inside of loop to be useful.
   *
   * @param dt the time taken during the last loop
   */ 
  void update(float dt);
  
};


#endif // CRUISE_CONTROL
