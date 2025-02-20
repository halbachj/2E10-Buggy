#ifndef PID_CONTROLLER_HPP
#define PID_CONTROLLER_HPP

/*
 * PID CONSTANTS STRUCT
 * This struct stores the PID constants in a neat way
 */
struct PIDConstants {
  const float Kp, Ki, Kd;  // Coefficient values
};


/*
 * PID CONTROLLER CLASS
 * The PID Controller class handles the necessary calculations associated with a PID controller.
 * It does not hanlde any control whatsoever and the obtained correaction values have to be applied 
 * by the associated driver itself.
 * Usage: 
 * Initialize with known Kp Ki Kd values as coefficients
 * Call update with the error term
 * Use result as the correction term to apply to the driver e.g. change the PWM signal
 */

class PIDController {
private:
  const PIDConstants& constants; 
  float lastError = 0;               // Error from previous calculation
  unsigned long prevTime;            // Time from last update
  float accumulated_error = 0;
  const float integralMax = 100.0;   // Example anti-windup limit
public:
  PIDController(const PIDConstants& constants);
  /*
   * Update method is called with an error term and returns a correction term to be applied to the driver
   */
  float update(float error, double dt);
};


#endif // PID_CONTROLLER_HPP
