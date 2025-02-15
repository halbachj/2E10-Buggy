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
    } else {
        this->echoDuration = micros() - startTime;  // Echo ended
    }
}

void UltrasonicSensor::update() {
  if(millis() - this->last_trigger > this->trigger_interval ) {
    this->trigger();
    this->last_trigger = millis();
  }
}


void UltrasonicSensor::setDetectionRange(unsigned short range) {
  this->detectionRange = range;
}


bool UltrasonicSensor::objectDetected() {
  return this->getReading() < this->detectionRange;
}

float UltrasonicSensor::getReading() {
  return echoDuration / 58.3;
}