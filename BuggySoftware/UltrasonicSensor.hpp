#ifndef ULTRASONIC_HPP
#define ULTRASONIC_HPP

#include "Arduino.h"
#include "PinSetup.hpp"
#include "MoveMeanFilter.hpp"


/**
 * @class UltrasonicSensor
 * @brief Handles ultrasonic sensor operations, including triggering and measuring distance.
 */
class UltrasonicSensor {
private:

  const UltrasonicSensorPinGroup& pinout; ///< Pin configuration for the ultrasonic sensor.
  const unsigned short triggerPulseLength = 10; ///< Trigger pulse duration in microseconds.

  const int trigger_interval = 20; ///< Minimum interval between triggers in milliseconds.
  volatile int echoDuration = 0; ///< Stores the duration of the received echo.
  unsigned short detectionRange = 20; ///< Detection range threshold in centimeters.

  volatile unsigned long startTime, endTime; ///< Variables for timing the echo measurement.
  volatile bool isReading = false; ///< Flag to indicate if the sensor is currently taking a reading

  static const size_t filter_window = 5;
  MoveMeanFilter<float, filter_window> filter = MoveMeanFilter<float, filter_window>();

  /**
   * @brief Sends a trigger pulse to initiate an ultrasonic measurement.
   */
  void trigger();
public:
  /**
   * @brief Constructs an UltrasonicSensor object with the given pin configuration.
   * @param pinout The pin configuration for the sensor.
   */
  UltrasonicSensor(const UltrasonicSensorPinGroup& pinout);
  
  /**
   * @brief Gets the latest distance measurement from the sensor.
   * @return Measured distance in centimeters.
   */
  float getReading();
  /**
   * @brief Sets the detection range for object detection.
   * @param range Detection range in centimeters.
   */
  void setDetectionRange(unsigned short range);
  /**
   * @brief Checks if an object is detected within the set range.
   * @return True if an object is detected, false otherwise.
   */
  bool objectDetected();
  /**
   * @brief Updates the sensor state, including triggering new measurements if necessary.
   */
  void update();
  /**
   * @brief Interrupt Service Routine for handling ultrasonic echo signal.
   */
  void ISR_UltrasonicEcho();
};


#endif //ULTRASONIC_HPP