#ifndef BUGGY_MOTOR_DRIVER_HPP
#define BUGGY_MOTOR_DRIVER_HPP

#include "PinSetup.hpp"
#include "PIDController.hpp"
#include <Arduino.h>

class MotorDriver {
private:
	const MotorPinGroup& pins;
  PIDController controller;

	unsigned int set_speed = 0;
	unsigned int measured_speed = 0;
  int pwm_cycle = 0;
	volatile unsigned long last_encoder_measurement = millis();
	volatile unsigned long current_encoder_measurement = millis();
  //volatile unsigned long degrees = 0;
  //unsigned long prevDegrees = 0;


  const unsigned short ticks = 4;                 // encoder ticks per rotations
  const unsigned short degPerTick = 360/ticks;    // ratio of degrees per tick

public:
	MotorDriver(const MotorPinGroup& pinGroup, const PIDConstants& constants);
	~MotorDriver();

	// Disable copying of this class
	MotorDriver(const MotorDriver& other) = delete;
	MotorDriver& operator=(const MotorDriver& other) = delete;

  void forward();
  void backward();
	void update(unsigned int dt);
  void pwmOverride(int pwm);
  void setSpeed(int speed);
  int getSpeed();
  
  void ISR_encoder_trigger();
};

#endif //BUGGY_MOTOR_DRIVER_HPP
