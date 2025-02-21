/**
 * @file BuggySoftware.ino
 * @author Johannes Halbach
 * @brief This is the main entry point for the Buggy software. @see setup @see loop
 */

#include "Arduino.h"
#include "Timer1.hpp"
#include "PinSetup.hpp"
#include "Logger.hpp"
#include "Buggy.hpp"
#include "PIDController.hpp"
#include "PacketFactory.hpp"

#define VERSION 1.1

namespace mcu {
LeanStreamIO logger(3);  // File descriptor 1 for Serial. Change to 2 for tcp
}
//using LogLevel = mcu::Logger::LogLevel;


/*
 * IMPORTANT TOP SECRET RESEARCH RESULTS XD
 *
 * ACCORDING TO ARDUINO PINMUX FOLLOWING PINS HAVE INTERRUPT CAPABILITY
 * SOME OF THOSE PINS HAVE OTHER EXTERNAL CAPABILITIES LIKE I2C OR ADC/DAC
 * LOOKING AT THE RA4M1 DATASHEET SOME SHARE THE SAME INTERRUPT REQUEST LINE
 *
 * PIN FUNCTIONALITY | SERIALPORT  ||     PURE DIGITAL PINS     ||    ANALOG PINS    ||  I2C PINS
 * ------------------+------+------++------+------+------+------++------+------+-----++------+------
 * ARDUINO PINOUT    | 0    |  1   || 2    | 3    | 6    | 8    || 15   | 16   | 17  || 18   | 19
 * ------------------+------+------++------+------+------+------++------+------+-----++------+------
 * RA4M1 PINOUT      | P301 | P302 || P104 | P105 | P111 | P304 || P000 | P001 | P002|| P101 | P100
 * INTERRUPT NUMBER  | IRQ6 | IRQ5 || IRQ1 | IRQ0 | IRQ4 | IRQ9 || IRQ6 | IRQ7 | IRQ2|| IRQ1 | IRQ2
 * 
 * Best choice for Interrupt pins is 2, 3, 6, 8 as they are not on the same interrupt request line and
 * all of them are pure digital pins, hence no overlap with I2C or ADC/DAC capabilities.
 * 
 * SOURCES:
 * https://github.com/arduino/ArduinoCore-renesas/blob/main/variants/UNOWIFIR4/pinmux.inc
 * https://www.renesas.com/en/document/dst/ra4m1-group-datasheet
 * https://docs.arduino.cc/resources/pinouts/ABX00087-full-pinout.pdf 
 *
 */

/// MOTOR PINPUTS
const MotorPinGroup leftMotorPinout = { 6, 7, 5, 2 };
const MotorPinGroup rightMotorPinout = { 10, 12, 11, 3 };

/// IR SENSORS
PIN_TYPE leftIrSensorPin = A1;
PIN_TYPE rightIrSensorPin = A0;

/// ULTRASONIC SENSOR
const UltrasonicSensorPinGroup ultrasonicSensorPinout = { 9, 8 };

/// PID CONSTANTS
const PIDConstants leftMotorPID = { 0.05f, 1.0, 0.0f };
//const PIDConstants leftMotorPID = {0.015625f, 0.00f, 0.015625f};
//const PIDConstants rightMotorPID = { 0.05f, 0.00f, 0.00f };
const PIDConstants rightMotorPID = {0.04f, 1.0f, 0.00f};
const PIDConstants lineFollowerPID = { 1.0f, 0.0f, 0.0f };

/// INSTANCES
MotorDriver leftMotor(leftMotorPinout, leftMotorPID);
MotorDriver rightMotor(rightMotorPinout, rightMotorPID);

IrSensor leftIrSensor(leftIrSensorPin);
IrSensor rightIrSensor(rightIrSensorPin);

UltrasonicSensor ultrasonicSensor(ultrasonicSensorPinout);

BuggyWiFi wifi;
TcpServer server = TcpServer();
TcpServer logging_server = TcpServer(44);

LineFollower lineFollower(leftMotor, rightMotor, leftIrSensor, rightIrSensor, lineFollowerPID);

Buggy buggy = Buggy(leftMotor, rightMotor, leftIrSensor, rightIrSensor, ultrasonicSensor, wifi, server, lineFollower);

// ISR
void ISR_left_motor();
void ISR_right_motor();
void ISR_ultrasonic_echo();

/**
 * @brief Setup method used for setting up the Arduino when it is started.
 * @details This sets up all the communication services as well. Besides 
 * the basic pin setup this includes the WiFi, Serial and TCP server.
 */
void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  mcu::logger << "INIT Start" << mcu::LeanStreamIO::endl;

  // start Timer 1
  BuggyTimer1::begin();

  // PIN SETUP
  pinMode(LED_BUILTIN, OUTPUT);  // WiFi LED

  // MOTOR PINS
  setupMotorPins(leftMotorPinout);
  setupMotorPins(rightMotorPinout);

  // ULTRASONIC PINS
  setupUltrasonicPins(ultrasonicSensorPinout);

  // Attatch motor interrupts
  attachInterrupt(digitalPinToInterrupt(leftMotorPinout.encoder), ISR_left_motor, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rightMotorPinout.encoder), ISR_right_motor, CHANGE);

  // Attatch Ultrasonic interrupt
  attachInterrupt(digitalPinToInterrupt(ultrasonicSensorPinout.echo_pin), ISR_ultrasonic_echo, CHANGE);

  // SETUP WIFI
  wifi.wifi_checks();
  wifi.setup_ap();
  wifi.printWiFiStatus();
  server.setup();
  mcu::logger << "INIT Done" << mcu::LeanStreamIO::endl;

  buggy.setState(LineFollowingState::instance());
  //leftMotor.setSpeed(1000);
  leftMotor.forward();
  rightMotor.forward();
  //rightMotor.setSpeed(1000);
}

unsigned long start_time, end_time;
double dt = 0.01;           //s so 10ms
uint8_t loop_duration = 5;  //ms at least

/**
 * @brief Main loop of the arduino. Called as often ass possible.
 * @details This function is used to update all members of the buggy
 * as often as possible. This method will call the Buggy update method
 * which intern updates the necessary components of the buggy dependent
 * on the state. The function also keeps track of execution time in ms,
 * which is passed onto the subsequent update functions if they need it.
 */
void loop() {
  start_time = micros();
  //wifi.update();
  buggy.update(dt);
  end_time = micros();
  dt = (end_time - start_time) / 1000000;
  //mcu::logger << String(end_time-start_time).c_str() << mcu::LeanStreamIO::endl;
  //delay(max(0, loop_duration - dt));
}


/**
 * @brief Interrupt service routine for the left motor.
 * @todo rewrite using Timer1 instead of millis. 
 */
void ISR_left_motor() {
  leftMotor.ISR_encoder_trigger();
}

/**
 * @brief Interrupt service routine for the right motor.
 * @todo rewrite using Timer1 instead of millis. 
 */
void ISR_right_motor() {
  rightMotor.ISR_encoder_trigger();
}

/**
 * @brief Interrupt service routine for the Ultrasonic sensor.
 * @todo rewrite using Timer1 instead of millis
 */
void ISR_ultrasonic_echo() {
  ultrasonicSensor.ISR_UltrasonicEcho();
}
