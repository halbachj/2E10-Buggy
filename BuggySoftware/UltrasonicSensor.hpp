#ifndef ULTRASONIC_HPP
#define ULTRASONIC_HPP

#include "Arduino.h"
#include "helpers.hpp"
#include "PinSetup.hpp"


/*
 *
 *  FUCK YEAH!! According to the Datasheet of the RA4M1 chip the pin P002 is connected to IRQ2.
 *  That pin is D17 on the arduino board. This effecively adds another interrupt pin to the Arduino.
 *  Honestly I will keep this a secret. Having three interrupt pins is pretty neat...
 */

/*
 * ULTRASONIC SENSOR CLASS
 * The ultrasonic sensor class provides an interface for reading distance measurements
 * using an ultrasonic sensor. It manages the trigger pulse required to initiate the measurement
 * and handles the echo signal to calculate the distance. The sensor uses a specific pinout
 * configuration and supports a trigger pulse length of 10 microseconds. The `getReading()` method
 * returns the current distance measurement, and the `ISR_UltrasonicEcho()` method is used
 * to handle the echo signal in an interrupt service routine for precise distance calculations.
 */
class UltrasonicSensor {
private:
  const UltrasonicSensorPinGroup& pinout;
  const unsigned short triggerPulseLength = 10; //10us trigger pulse;

  const int trigger_interval = 10; //ms
  volatile int echoDuration = 0;
  unsigned short detectionRange = 20;

  
  volatile unsigned long startTime, endTime;
  unsigned long last_trigger = 0;

  void trigger();
public:
  UltrasonicSensor(const UltrasonicSensorPinGroup& pinout);
  
  float getReading();
  void setDetectionRange(unsigned short range);
  bool objectDetected();
  void update();

  void ISR_UltrasonicEcho();

};


#endif //ULTRASONIC_HPP