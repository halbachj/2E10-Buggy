#include "IrSensor.hpp"
#include "Arduino.h"

IrSensor::IrSensor(const pin_size_t input_pin): input_pin(input_pin) {}

void IrSensor::setCalibrationRange(int min, int max) {
  this->min=min;
  this->max=max;
}

int IrSensor::getReading() {
  int reading = this->getManualReading();
  return map(reading, 0, 1023, this->min, this->max);
}

int IrSensor::getManualReading() {
  return analogRead(this->input_pin);
}
