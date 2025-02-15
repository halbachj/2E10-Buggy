#include "Arduino.h"
#include "PinSetup.hpp"
#include "Logger.hpp"
#include "Buggy.hpp"
#include "PIDController.hpp"
#include "PacketFactory.hpp"

#define VERSION 1.1

namespace mcu {
  LeanStreamIO logger(3); // File descriptor 1 for Serial. Change to 2 for tcp
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

// MOTOR PINOUT
const MotorPinGroup leftMotorPinout = {6,7,5,2};
const MotorPinGroup rightMotorPinout = {10,12,11,3};

// IR SENSORS
PIN_TYPE leftIrSensorPin = A1;
PIN_TYPE rightIrSensorPin = A0;

// ULTRASONIC SENSOR
const UltrasonicSensorPinGroup ultrasnoicSensorPinout = {9,8};

// PID CONSTANTS
const PIDConstants leftMotorPID = {0.05f, 0.00f, 0.00f};
const PIDConstants rightMotorPID = {0.05f, 0.00f, 0.00f};
const PIDConstants lineFollowerPID = {1.0f, 0.0f, 0.0f};

// INSTANCES
MotorDriver leftMotor(leftMotorPinout, leftMotorPID);
MotorDriver rightMotor(rightMotorPinout, rightMotorPID);

IrSensor leftIrSensor(leftIrSensorPin);
IrSensor rightIrSensor(rightIrSensorPin);

UltrasonicSensor ultrasonicSensor(ultrasnoicSensorPinout);

BuggyWiFi wifi;
TcpServer server = TcpServer();
TcpServer logging_server = TcpServer(44);

LineFollower lineFollower(leftMotor, rightMotor, leftIrSensor, rightIrSensor, lineFollowerPID);

// INITIALIZE BUGGY
Buggy buggy = Buggy(leftMotor, rightMotor, leftIrSensor, rightIrSensor, ultrasonicSensor, wifi, server, lineFollower);

// ISR
void ISR_left_motor();
void ISR_right_motor();
void ISR_ultrasonic_echo();


void setup() {
  Serial.begin(115200);
  while(!Serial);
  mcu::logger << "INIT Start" << mcu::LeanStreamIO::endl;
  
  // PIN SETUP
  pinMode(LED_BUILTIN, OUTPUT); // WiFi LED

  // MOTOR PINS
  setupMotorPins(leftMotorPinout);
  setupMotorPins(rightMotorPinout);

  // ULTRASONIC PINS
  setupUltrasonicPins(ultrasnoicSensorPinout);

  // Attatch motor interrupts
  attachInterrupt(digitalPinToInterrupt(leftMotorPinout.encoder), ISR_left_motor, FALLING);
  attachInterrupt(digitalPinToInterrupt(rightMotorPinout.encoder), ISR_right_motor, FALLING);

  // Attatch Ultrasonic interrupt
  attachInterrupt(digitalPinToInterrupt(ultrasnoicSensorPinout.echo_pin), ISR_ultrasonic_echo, CHANGE);
  
  // SETUP WIFI
  wifi.wifi_checks();
  wifi.setup_ap();
  wifi.printWiFiStatus();
  server.setup();
  mcu::logger << "INIT Done" << mcu::LeanStreamIO::endl;

  //buggy.setState(IdleState::instance());
}

unsigned long start_time, end_time, first_time = millis();
unsigned int dt = 10;

// MAIN LOOP
void loop() {
  start_time = millis();
  wifi.update();
  buggy.update(dt);
  end_time = millis();
  dt = end_time - start_time;
}


// ISR IMPLEMENTATIONS
void ISR_left_motor() {
  leftMotor.ISR_encoder_trigger();
}

void ISR_right_motor() {
  rightMotor.ISR_encoder_trigger();
}

void ISR_ultrasonic_echo() {
  ultrasonicSensor.ISR_UltrasonicEcho();
}
