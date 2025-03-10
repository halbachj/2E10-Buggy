#ifndef BUGGY_MOTOR_DRIVER_HPP
#define BUGGY_MOTOR_DRIVER_HPP

#include "PinSetup.hpp"
#include "PIDController.hpp"
#include "MoveMeanFilter.hpp"

/**
 * @class MotorDriver
 * @brief Controls a motor using PWM signals and a PID controller.
 *
 * This class manages motor movement, speed control, and distance measurement
 * using an encoder. It includes PID-based speed regulation and provides methods
 * for setting speed, overriding PWM, and measuring distance traveled.
 */
class MotorDriver {
private:
	const MotorPinGroup& pins;
  PIDController controller;
  
  static const size_t filter_window = 200;
  MoveMeanFilter<unsigned int, filter_window> filter = MoveMeanFilter<unsigned int, filter_window>();
  const unsigned short ticks = 8;                 // encoder ticks per rotations
  const unsigned short degPerTick = 360/ticks;    // ratio of degrees per tick

  const float wheelCircumference = 0.025 * 2.54 * 3.1415926 * 0.125; // Wheel Circumference in m

	unsigned int set_speed = 0;
	unsigned int measured_speed = 0;

	volatile unsigned long last_encoder_measurement = millis();
	volatile unsigned long current_encoder_measurement = millis();
  volatile unsigned long degrees = 0;

  unsigned int applied_pwm = 0;

public:
  /**
   * @brief Constructs a MotorDriver object.
   * @param pinGroup Reference to the motor pin configuration.
   * @param constants PID controller constants.
   */
	MotorDriver(const MotorPinGroup& pinGroup, const PIDConstants& constants);
	~MotorDriver();

	/**
   * @brief Deleted copy constructor to prevent copying.
   */
	MotorDriver(const MotorDriver& other) = delete;
  /**
   * @brief Deleted copy assignment operator to prevent copying.
   */
	MotorDriver& operator=(const MotorDriver& other) = delete;

  /**
   * @brief Sets the motor direction to forward by writing to the H-Bridge.
   */
  void forward();
  /**
   * @brief Sets the motor direction to backward by writing to the H-Bridge.
   */
  void backward();
  /**
   * @brief Updates the motor control logic, including PID calculations.
   * @param dt Time step for the update (in seconds).
   */
	void update(double dt);

  /**
    * @brief Overrides the PWM signal applied to the motor.
    * @param pwm PWM value to apply.
    */
  void pwmOverride(int pwm);
  /**
   * @brief Sets the desired speed of the motor.
   * @param speed Target speed (units depend on implementation).
   */
  void setSpeed(int speed);

  /**
   * @brief Resets the recorded distance traveled.
   */
  void resetDistance();
  /**
   * @brief Gets the last applied PWM value.
   * @return Last PWM value applied to the motor.
   */
  int getPWM();

  /**
   * @brief Gets the total distance traveled by the motor.
   * @return Distance traveled in meters.
   */
  float getDistanceTraveled();

  /**
   * @brief Gets the current speed of the motor.
   * @return Current speed (deg per second).
   */
  int getSpeed();
  
  /**
   * @brief Interrupt Service Routine (ISR) for encoder signal processing.
   *
   * This function should be called in response to an encoder interrupt to
   * update speed and distance measurements.
   */
  void ISR_encoder_trigger();
};

#endif //BUGGY_MOTOR_DRIVER_HPP
