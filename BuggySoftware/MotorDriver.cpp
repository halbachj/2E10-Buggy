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
  this->encoder_tirggered = true;
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
  this->applied_pwm = pwm;
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
  
  noInterrupts();
  measurement_diff = this->current_encoder_measurement - this->last_encoder_measurement;
  interrupts();

  // measurement dt is 3000 increments per ms therefore divide by 3000 to get ms

  if (!this->encoder_tirggered) {
    this->measured_speed = 0;
  } else {
    this->measured_speed = (this->degPerTick * 2.0 * 1000) / (float)measurement_diff;
  }

  this->measured_speed = this->filter.update(this->measured_speed);

  if (this->last_encoder_measurement + measurement_diff * 3 < BuggyTimer1::counter) this->measured_speed = 0; // there should have been an encoder pulse by now...
  logger << logLevel::DEBUG;
  logger << this->measured_speed << ",";
  logger << this->set_speed << ",";

  error = float(this->set_speed) - float(this->measured_speed);

  logger << error << ",";

  correction = this->controller.update(error, dt);
  logger << correction << ",";
  
  pwm = constrain(round(correction), 0, 255);
  analogWrite(this->pins.pulse, abs(pwm));
  logger << pwm;
  logger << EmbeddedLogger::endl;

}
