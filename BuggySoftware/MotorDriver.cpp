#include "MotorDriver.hpp"
#include "math.h"
#include "EmbeddedLogger.hpp"
#include "Timer1.hpp"

// Measured max speed is 1496 deg/s - 4.15 rps

using EmbeddedLogger::logger;
using logLevel = EmbeddedLogger::LogLevel;

MotorDriver::MotorDriver(const MotorPinGroup& pins, const PIDConstants& constants) : pins(pins), controller(constants) {
  this->forward();
}

MotorDriver::~MotorDriver() {}

void MotorDriver::ISR_encoder_trigger() {
  this->degrees += 45;
  this->last_encoder_measurement = this->current_encoder_measurement;
  this->current_encoder_measurement = BuggyTimer1::counter;
}

void MotorDriver::forward() {
  digitalWrite(this->pins.dir_a, LOW);
  digitalWrite(this->pins.dir_b, HIGH);
}

void MotorDriver::backward() {
  digitalWrite(this->pins.dir_a, HIGH);
  digitalWrite(this->pins.dir_b, LOW);
}

void MotorDriver::setSpeed(int speed) {
  this->set_speed = speed;
}

void MotorDriver::pwmOverride(int pwm) {
  analogWrite(this->pins.pulse, pwm);
}

float MotorDriver::getDistanceTraveled() {
  return (this->degrees / 45) * this->wheelCircumference;
}

int MotorDriver::getPWM() {
  return this->applied_pwm;
}

int MotorDriver::getSpeed() {
  return this->measured_speed;
}

void MotorDriver::resetDistance()
{
  this->degrees = 0;
}

void MotorDriver::update(double dt) {
  int measurement_diff, pwm;
  float error, correction;
  logger << logLevel::DEBUG;
  
  noInterrupts();
  measurement_diff = this->current_encoder_measurement - this->last_encoder_measurement;
  interrupts();

  logger << measurement_diff << ",";

  // measurement dt is 3000 increments per ms therefore divide by 3000 to get ms

  this->measured_speed = (this->degPerTick * 2.0 * 1000) / (float)measurement_diff;
  logger << this->measured_speed << ",";
  this->filter.push(this->measured_speed);
  this->measured_speed = this->filter.getMean();

  if (this->last_encoder_measurement + measurement_diff * 3 < BuggyTimer1::counter) this->measured_speed = 0; // there should have been an encoder pulse by now...
  
  logger << this->measured_speed << ",";
  logger << this->set_speed << ",";

  error = float(this->set_speed) - float(this->measured_speed);

  logger << error << ",";

  correction = this->controller.update(error, dt);
  logger << correction;

  pwm = constrain(round(correction), 0, 255);
  analogWrite(this->pins.pulse, abs(pwm));

  logger << EmbeddedLogger::endl;

}
