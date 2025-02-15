#ifndef IR_SENSOR_HPP
#define IR_SENSOR_HPP

#include "helpers.hpp"


/*
 * IR SENSOR CLASS
 * The IR sensor class provides an interface for reading data from an infrared sensor.
 * It allows for easy calibration of the sensor range and provides methods to read
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
  IrSensor(PIN_TYPE input_pin);
  void setCalibrationRange(int min, int max);
  int getReading();
  int getManualReading();
};


#endif //IR_SENSOR_HPP
