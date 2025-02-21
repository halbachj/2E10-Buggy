#ifndef BUGGY_MOTOR_DRIVER_HPP
#define BUGGY_MOTOR_DRIVER_HPP

#include "PinSetup.hpp"
#include "PIDController.hpp"
#include "MoveMeanFilter.hpp"
#include <Arduino.h>

class MotorDriver {
private:
	const MotorPinGroup& pins;
  PIDController controller;

  const size_t filter_window = 200;
  MoveMeanFilter<int, filter_window> filter<int, filter_window>();

  const unsigned short ticks = 8;                 // encoder ticks per rotations
  const unsigned short degPerTick = 360/ticks;    // ratio of degrees per tick

  const float wheelCircumference = 0.025 * 2.54 * 3.1415926; // Wheel Circumference in m

	unsigned int set_speed = 0;
	unsigned int measured_speed = 0;

	volatile unsigned long last_encoder_measurement = millis();
	volatile unsigned long current_encoder_measurement = millis();
  volatile unsigned long degrees = 0;

  unsigned int applied_pwm = 0;

public:
	MotorDriver(const MotorPinGroup& pinGroup, const PIDConstants& constants);
	~MotorDriver();

	// Disable copying of this class
	MotorDriver(const MotorDriver& other) = delete;
	MotorDriver& operator=(const MotorDriver& other) = delete;

  void forward();
  void backward();
	void update(double dt);
  void pwmOverride(int pwm);
  void setSpeed(int speed);
  int getPWM();
  float getDistanceTraveled();
  int getSpeed();
  
  void ISR_encoder_trigger();
};

#endif //BUGGY_MOTOR_DRIVER_HPP
