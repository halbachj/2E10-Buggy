#ifndef IR_SENSOR_HPP
#define IR_SENSOR_HPP

#include "helpers.hpp"


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
  PIN_TYPE input_pin;
  int min=0, max=1023;
public:
  /**
   * @brief Will construct the IrSensor using the Pins.
   *
   * @param input_pin is the pin used as the input pin for the IrSensor on the Arduino
   */
  IrSensor(PIN_TYPE input_pin);
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
};


#endif //IR_SENSOR_HPP
