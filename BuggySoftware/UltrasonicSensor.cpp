#include "UltrasonicSensor.hpp"


UltrasonicSensor::UltrasonicSensor(const UltrasonicSensorPinGroup& pinout): pinout(pinout) {

}

void UltrasonicSensor::trigger() {
    digitalWrite(pinout.trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(pinout.trig_pin, HIGH);
    delayMicroseconds(this->triggerPulseLength);
    digitalWrite(pinout.trig_pin, LOW);

}

void UltrasonicSensor::ISR_UltrasonicEcho() {
    
    if (digitalRead(pinout.echo_pin)) {
        this->startTime = micros();  // Echo started
        this->isReading = true;
    } else {
        this->endTime = micros();  // Echo ended
        this->isReading = false;
    }
}

void UltrasonicSensor::update() {
  if( !this->isReading && this->endTime + (this->trigger_interval * 1000) < micros() ) {
    this->echoDuration = this->endTime - this->startTime;
    noInterrupts();
    this->filter.push(echoDuration / 58.3);  // Disable interrupts while reading from the sensor
    interrupts();
    this->trigger();
  }
}


void UltrasonicSensor::setDetectionRange(unsigned short range) {
  this->detectionRange = range;
}


bool UltrasonicSensor::objectDetected() {
  return this->getReading() < this->detectionRange;
}

float UltrasonicSensor::getReading() {
  return this->filter.getMean();
}