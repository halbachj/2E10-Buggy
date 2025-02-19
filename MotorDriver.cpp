#include "MotorDriver.hpp"
#include "Logger.hpp"
#include "Timer1.hpp"

// Measured max speed is 1496 deg/s - 4.15 rps


MotorDriver::MotorDriver(const MotorPinGroup& pins, const PIDConstants& constants) : pins(pins), controller(constants) {
  this->forward();
}

MotorDriver::~MotorDriver() {
  
}

void MotorDriver::ISR_encoder_trigger() {
  this->last_encoder_measurement = this->current_encoder_measurement;
  this->current_encoder_measurement = BuggyTimer1::counter;
  //this->last_encoder_measurement = this->current_encoder_measurement;
  //this->current_encoder_measurement = millis();
  //this->degrees += 90;
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

int MotorDriver::getSpeed() {
  return this->measured_speed;
}

void MotorDriver::update(unsigned int dt) {

  
  //mcu::logger << "dt" << String(dt).c_str() << " deg " << String(this->degrees).c_str() << " diff " << String(this->degrees - this->prevDegrees).c_str() << mcu::LeanStreamIO::endl;
  /*noInterrupts();
  this->measured_speed = ((this->degrees - this->prevDegrees) * 1000)/(dt);
  this->prevDegrees = this->degrees;
  interrupts();
  */

  noInterrupts();
  int measurement_diff = this->current_encoder_measurement - this->last_encoder_measurement;
  interrupts();
  mcu::logger <<String(micros()).c_str() << ",";
  mcu::logger <<String(this->current_encoder_measurement).c_str() << ",";
  mcu::logger <<String(this->last_encoder_measurement).c_str() << ",";
  mcu::logger <<String(measurement_diff).c_str() << ",";
  // measurement dt is 3000 increments per ms therefore divide by 3000 to get ms

  this->measured_speed = (this->degPerTick * 2 * 1000UL) / float(measurement_diff);
  
  if (this->last_encoder_measurement + measurement_diff * 3 < BuggyTimer1::counter) this->measured_speed = 0; // there should have been an encoder pulse by now...
  
  mcu::logger <<String(this->measured_speed).c_str() << ",";
  mcu::logger << String(this->set_speed).c_str() << ",";

  bool running_direction = this->pwm_cycle > 0; // true if forward

  float error = float(this->set_speed) - float(this->measured_speed);

  mcu::logger << String(error).c_str() << ",";

  float correction = this->controller.update(error, float(dt));
  mcu::logger << String(correction).c_str() << ",";

  this->pwm_cycle += correction;

  mcu::logger << String(this->pwm_cycle).c_str() << ",";

  this->pwm_cycle = constrain(this->pwm_cycle, 0, 255);
  analogWrite(this->pins.pulse, abs(this->pwm_cycle));

  mcu::logger << String(this->pwm_cycle).c_str();

  mcu::logger << mcu::LeanStreamIO::endl;

  // Change direction if pwm is negative and not already running backwards
  /*
  if(this->pwm_cycle < 0 && running_direction)
    this->backward();
  // Change direction if pwm is positive and not already running forwards
  if(this->pwm_cycle > 0 && !running_direction);
    this->forward();
  */
}
