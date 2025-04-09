#ifndef IR_SENSOR_HPP
#define IR_SENSOR_HPP

#include "Arduino.h"

/**
 * @class IrSensor
 * @brief The IR sensor class provides an interface for reading data from an infrared sensor.
 * @details It allows for easy calibration of the sensor range and provides methods to read
 * values from the sensor. The sensor reading can be fetched either automatically
 * with the `getReading()` method or manually using the `getManualReading()` method.
 * The class also supports calibrating the sensor's minimum and maximum values
 * using the `setCalibrationRange()` method.
 */

class IrSensor {
private:
  const pin_size_t input_pin;
  int min=0, max=1023;
  unsigned int threshold = 100;
public:
  /**
   * @brief Will construct the IrSensor using the Pins.
   *
   * @param input_pin is the pin used as the input pin for the IrSensor on the Arduino
   */
  IrSensor(const pin_size_t input_pin);
  /**
   * @brief Will set the calibration range of the IrSensor.
   *
   * @param min the minimum of the range
   * @param max the maximum of the range
   */
  void setCalibrationRange(int min, int max);
  /**
   * @brief Takes a calibrated reading mapped by the calibration range.
   * 
   * @returns the calibrated rading
   */
  int getReading();
  /**
   * @brief Takes a raw manual reading straight from the sensor.
   * 
   * @returns the raw analog reading.
   */
  int getManualReading();
  /**
   * @brief Returns the reading of the sensor as a boolean based on a threshold value.
   * 
   * @returns true if the sensor reads a high value
   */
  bool isHigh();
  /**
   * @brief Returns the reading of the sensor as a boolean based on a threshold value.
   * 
   * @returns true if the sensor reads a high value
   */
  bool isLow();
  /**
   * @brief Setter for the threshold value.
   * 
   * @param value the threshold value to be set.
   */
  void setThreshold(unsigned int value);
};


#endif //IR_SENSOR_HPP
