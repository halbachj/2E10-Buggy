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
  UltrasonicSensor& ussensor;
  PIDController controller; // Initialize the PID with the right coefficients
  int target_distance = 20; // cm
  int max_speed = 600;
  
public:
  /**
   * @brief Construct the Cruise controller
   *
   * @param lineFollower the line follower helper class
   */
  CruiseControl(UltrasonicSensor& ussensor, LineFollower& lineFollower, const PIDConstants& constansts);
  /**
   * @brief Will read and adapt the line follower speed at each call. Needs to be called inside of loop to be useful.
   *
   * @param dt the time taken during the last loop
   */ 
  float update(double dt);
  
};


#endif // CRUISE_CONTROL
