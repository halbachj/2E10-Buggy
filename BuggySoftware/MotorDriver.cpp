#include "MotorDriver.hpp"
#include "math.h"
#include "EmbeddedLogger.hpp"
#include "Timer1.hpp"

// Measured max speed is 1496 deg/s - 4.15 rps

using EmbeddedLogger::logger;

MotorDriver::MotorDriver(const MotorPinGroup& pins, const PIDConstants& constants) : pins(pins), controller(constants) {
  this->forward();
}

MotorDriver::~MotorDriver() {
  
}

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


  noInterrupts();
  measurement_diff = this->current_encoder_measurement - this->last_encoder_measurement;
  interrupts();

  logger <<String(micros()).c_str() << ",";
  logger <<String(this->current_encoder_measurement).c_str() << ",";
  logger <<String(this->last_encoder_measurement).c_str() << ",";
  logger <<String(measurement_diff).c_str() << ",";
  // measurement dt is 3000 increments per ms therefore divide by 3000 to get ms

  this->measured_speed = (this->degPerTick * 2 * 1000UL) / float(measurement_diff);
  
  if (this->last_encoder_measurement + measurement_diff * 3 < BuggyTimer1::counter) this->measured_speed = 0; // there should have been an encoder pulse by now...
  
  logger <<String(this->measured_speed).c_str() << ",";
  logger << String(this->set_speed).c_str() << ",";

  error = float(this->set_speed) - float(this->measured_speed);

  logger << String(error).c_str() << ",";

  correction = this->controller.update(error, dt);
  logger << String(correction).c_str() << ",";

  //logger << String(this->pwm_cycle).c_str() << ",";

  pwm = constrain(round(correction), 0, 255);
  analogWrite(this->pins.pulse, abs(pwm));



  // Change direction if pwm is negative and not already running backwards
  /*
  if(this->pwm_cycle < 0 && running_direction)
    this->backward();
  // Change direction if pwm is positive and not already running forwards
  if(this->pwm_cycle > 0 && !running_direction);
    this->forward();
  */
}
