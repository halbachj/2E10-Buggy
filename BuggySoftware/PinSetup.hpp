#ifndef BUGGY_PIN_SETUP_HPP
#define BUGGY_PIN_SETUP_HPP

#include "helpers.hpp"
#include <Arduino.h>

/**
 * @struct MotorPinGroup
 * @brief Represents a set of motor control pins.
 */
struct MotorPinGroup {
    PIN_TYPE dir_a = 0;   ///< Direction pin A.
    PIN_TYPE dir_b = 0;   ///< Direction pin B.
    PIN_TYPE pulse = 0;   ///< PWM pulse pin.
    PIN_TYPE encoder = 0; ///< Encoder signal pin.
};

/**
 * @brief Configures the motor control pins.
 * @param group The MotorPinGroup containing the pin assignments.
 */
void setupMotorPins(const MotorPinGroup& group);


/**
 * @struct UltrasonicSensorPinGroup
 * @brief Represents a set of ultrasonic sensor control pins.
 */
struct UltrasonicSensorPinGroup {
    PIN_TYPE trig_pin = 0; ///< Trigger pin for the ultrasonic sensor.
    PIN_TYPE echo_pin = 0; ///< Echo pin for the ultrasonic sensor.
};

/**
 * @brief Configures the ultrasonic sensor control pins.
 * @param group The UltrasonicSensorPinGroup containing the pin assignments.
 */
void setupUltrasonicPins(const UltrasonicSensorPinGroup& group);

#endif //BUGGY_PIN_SETUP_HPP
