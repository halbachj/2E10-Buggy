/**
 * @file BuggySoftware.ino
 * @author Johannes Halbach
 * @brief This is the main entry point for the Buggy software. @see setup @see loop
 */

#include "Arduino.h"
#include "Timer1.hpp"
#include "PinSetup.hpp"
#include "EmbeddedLogger.hpp"
#include "SerialWirelessLogger.hpp"
#include "Buggy.hpp"
#include "PIDController.hpp"
#include "PacketFactory.hpp"
#include "Matrix.hpp"

#define VERSION 1.4

TcpServer logging_server = TcpServer(44);

namespace EmbeddedLogger {
  //SerialWirelessLogger target = SerialWirelessLogger(logging_server);
  SerialLogger target = SerialLogger();
  Logger logger(&target);
}

using EmbeddedLogger::logger;

/// @todo Write a scheduler service to delegate reoccuring tasks at specific intervals.

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
const pin_size_t leftIrSensorPin = A1;
const pin_size_t rightIrSensorPin = A0;

/// ULTRASONIC SENSOR
const UltrasonicSensorPinGroup ultrasonicSensorPinout = { 9, 8 };

///LED MATRIX
Matrix ledMatrix;

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
  //while (!Serial) yield();
  logger << "INIT Start" << EmbeddedLogger::endl;

  // start Timer 1
  BuggyTimer1::begin();

  // PIN SETUP
  pinMode(LED_BUILTIN, OUTPUT);  ///< @todo Make this part of the WiFi class
  

  // MOTOR PINS
  setupMotorPins(leftMotorPinout);  ///< @todo make this part of MotorDriver begin
  setupMotorPins(rightMotorPinout); ///< @todo make this part of MotorDriver begin

  // ULTRASONIC PINS
  setupUltrasonicPins(ultrasonicSensorPinout); ///< @todo make this part of UltrasonicSensor begin

  // LED MATRIX
  ledMatrix.begin(); ///< @todo make this part of the matrix class
  // Attatch motor interrupts
  attachInterrupt(digitalPinToInterrupt(leftMotorPinout.encoder), ISR_left_motor, CHANGE);   ///< @todo make this part of MotorDriver begin
  attachInterrupt(digitalPinToInterrupt(rightMotorPinout.encoder), ISR_right_motor, CHANGE); ///< @todo make this part of MotorDriver begin

  // Attatch Ultrasonic interrupt
  attachInterrupt(digitalPinToInterrupt(ultrasonicSensorPinout.echo_pin), ISR_ultrasonic_echo, CHANGE); ///< @todo make this part of UltrasonicSensor begin

  // SETUP WIFI
  wifi.wifi_checks(); ///< @todo make this part of BuggyWiFi begin
  wifi.setup_ap();    ///< @todo make this part of BuggyWiFi begin
  wifi.printWiFiStatus(); ///< @todo make this part of BuggyWiFi begin
  server.setup();         ///< @todo rename this to server.begin

  logger << "INIT Done" << EmbeddedLogger::endl;

  buggy.setState(LineFollowingState::instance());  ///< @todo either remove this from the buggy constructor or remove it here
  leftMotor.forward();  ///< @todo either remove this from the constructor or remove it here 
  rightMotor.forward(); ///< @todo either remove this from the constructor or remove it here
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
  buggy.update(dt);
  end_time = micros();
  dt = (end_time - start_time) / 1000000;

  //mcu::logger << String(end_time-start_time).c_str() << mcu::LeanStreamIO::endl;
  //delay(max(0, loop_duration - dt));
}


/**
 * @brief Interrupt service routine for the left motor.
 */
void ISR_left_motor() {
  leftMotor.ISR_encoder_trigger();
}

/**
 * @brief Interrupt service routine for the right motor.
 */
void ISR_right_motor() {
  rightMotor.ISR_encoder_trigger();
}

/**
 * @brief Interrupt service routine for the Ultrasonic sensor.
 */
void ISR_ultrasonic_echo() {
  ultrasonicSensor.ISR_UltrasonicEcho();
}
