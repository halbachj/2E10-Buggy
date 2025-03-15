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
#include "CruiseControl.hpp"

#define VERSION 1.4

using EmbeddedLogger::logger;
using logLevel = EmbeddedLogger::LogLevel;

TcpServer logging_server = TcpServer(44);

namespace EmbeddedLogger {
  //SerialWirelessLogger target = SerialWirelessLogger(logging_server);
  SerialLogger target = SerialLogger();
  Logger logger(&target);
}

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
const MotorPinGroup rightMotorPinout = { 10, 11, 12, 3 };

/// IR SENSORS
const pin_size_t leftIrSensorPin = A1;
const pin_size_t rightIrSensorPin = A0;

/// ULTRASONIC SENSOR
const UltrasonicSensorPinGroup ultrasonicSensorPinout = { 9, 8 };

///LED MATRIX
Matrix ledMatrix;

/// PID CONSTANTS
const PIDConstants leftMotorPID = { 1.0f, 1.0, 0.0f };
const PIDConstants rightMotorPID = {0.055f, 1.0f, 0.0f};
const PIDConstants lineFollowerPID = { 1.0f, 0.0f, 0.0f };
const PIDConstants cruiseControlPID = { 1.0f, 0.0f, 0.0f };

/// INSTANCES
MotorDriver leftMotor(leftMotorPinout, leftMotorPID);
MotorDriver rightMotor(rightMotorPinout, rightMotorPID);

IrSensor leftIrSensor(leftIrSensorPin);
IrSensor rightIrSensor(rightIrSensorPin);

UltrasonicSensor ultrasonicSensor(ultrasonicSensorPinout);

BuggyWiFi wifi;
TcpServer server = TcpServer();

LineFollower lineFollower(leftMotor, rightMotor, leftIrSensor, rightIrSensor, lineFollowerPID);
CruiseControl cruiseControl(ultrasonicSensor, lineFollower, cruiseControlPID);

Buggy buggy = Buggy(leftMotor, rightMotor, leftIrSensor, rightIrSensor, ultrasonicSensor, wifi, server, lineFollower, cruiseControl);

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
  logger.setLogLevel(logLevel::INFO);
  while (!Serial) yield();
  logger << logLevel::INFO << "INIT Start" << EmbeddedLogger::endl;

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

  // LED MATRIX
  ledMatrix.begin(); ///< @todo make this part of the matrix class
  
  // SETUP WIFI
  wifi.wifi_checks(); ///< @todo make this part of BuggyWiFi begin
  wifi.setup_ap();    ///< @todo make this part of BuggyWiFi begin
  wifi.printWiFiStatus(); ///< @todo make this part of BuggyWiFi begin
  server.setup();         ///< @todo rename this to server.begin

  logger << logLevel::INFO << "INIT Done" << EmbeddedLogger::endl;

  buggy.setState(JustDriveState::instance());
  leftMotor.forward();
  rightMotor.forward();
  delay(1000 * 10);  
}

unsigned long start_time, end_time;
double dt;                     //s
uint8_t loop_duration = 5e-6;  //s at least

unsigned long initial_time = millis();
unsigned int min_pwm = 0, max_pwm = 255;
unsigned long min_time = 500, max_time=5000;
unsigned long wait_time = 1000; 

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
  //buggy.update(dt);

  logger << start_time << ",";
  if (millis() - initial_time > wait_time) {
    wait_time = random(min_time, max_time);
    leftMotor.pwmOverride(random(min_pwm, max_pwm));
    initial_time = millis();
  }

  /*for (int i=0;i<255;i++) {
    leftMotor.pwmOverride(i);
    logger << "PWM SIGNAL: " << i << EmbeddedLogger::endl;
    delay(1000);
    leftMotor.pwmOverride(0);
    delay(1000);
  }
  logger << "FINISHED TEST" <<< EmbeddedLogger::endl;*/
  end_time = micros();
  dt = (end_time - start_time) / 1000000;
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
