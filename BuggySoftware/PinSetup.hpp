#ifndef BUGGY_PIN_SETUP_HPP
#define BUGGY_PIN_SETUP_HPP

#include "helpers.hpp"
#include <Arduino.h>

/*
 * MOTOR PINS
 * Each motor has two direction pins, one speed pin and one encoder pin
 * 
 * setupMotorPins takes a MotorPinGroup and does the required setup for the pins
 */
struct MotorPinGroup {
  PIN_TYPE dir_a = 0;
  PIN_TYPE dir_b = 0;
  PIN_TYPE pulse = 0;
  PIN_TYPE encoder = 0;
};

void setupMotorPins(const MotorPinGroup& group);


/*
 * ULTRANSONIC SENSOR PINS
 * The ultrasonic sensor has two pins. One pin triggers the echo pulse and the returns the range as a pulse
 * setupUltrasnoicPins takes a Ultrasonic pin group and does the required setup for the pins;
 */

 struct UltrasonicSensorPinGroup {
  PIN_TYPE trig_pin = 0;
  PIN_TYPE echo_pin = 0;
 };

void setupUltrasonicPins(const UltrasonicSensorPinGroup& group);

#endif //BUGGY_PIN_SETUP_HPP
