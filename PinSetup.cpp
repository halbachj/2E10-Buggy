#include "PinSetup.hpp"


/*
 * Setup pins for motor
 */
void setupMotorPins(const MotorPinGroup& group) {
  pinMode(group.dir_a, OUTPUT);
  pinMode(group.dir_b, OUTPUT);
  pinMode(group.pulse, OUTPUT);
  pinMode(group.encoder, INPUT_PULLUP); // Is grounded when magnet is detected
}

/*
 * Setup pins Ultrasonic sensor
 */
void setupUltrasonicPins(const UltrasonicSensorPinGroup& group) {
  pinMode(group.trig_pin, OUTPUT);
  pinMode(group.echo_pin, INPUT_PULLUP);
}