#include "Buggy.hpp"

Buggy::Buggy(MotorDriver& leftMotor, MotorDriver& rightMotor, IrSensor& leftIrSensor, IrSensor& rightIrSensor,
   UltrasonicSensor& ultrasonicSensor, BuggyWiFi& wifi, TcpServer& server, LineFollower& lineFollower):
   leftMotor(leftMotor), rightMotor(rightMotor), leftIrSensor(leftIrSensor), rightIrSensor(rightIrSensor), ultrasonicSensor(ultrasonicSensor),
   wifi(wifi), server(server), lineFollower(lineFollower), currentState(&IdleState::instance()) {
  
}

void Buggy::handleCommand(CommandPacket command) {
  mcu::logger << String(command.type).c_str() << mcu::LeanStreamIO::endl;
  switch (command.type) {
    case CommandType::START:
      mcu::logger << "START COMMAND" << mcu::LeanStreamIO::endl;
      this->setState(LineFollowingState::instance());
      break;
    case CommandType::STOP:
      this->setState(IdleState::instance());
      mcu::logger << "STOP COMMAND" << mcu::LeanStreamIO::endl;
      break;
    default:
      break;
  }
}

void Buggy::handleControlPacket(ControlPacket control) {
  int max_speed = 255;
  float leftSpeed, rightSpeed, set_speed;

  set_speed = max_speed * -1 * control.y;
  if (set_speed < 0) {
    this->leftMotor.backward();
    this->rightMotor.backward();
  }else {
    this->leftMotor.forward();
    this->rightMotor.forward();
  }

  leftSpeed = set_speed * (control.x+1);
  rightSpeed = set_speed * (control.x-1);

  mcu::logger << String(leftSpeed).c_str() << mcu::LeanStreamIO::endl;
  mcu::logger << String(rightSpeed).c_str() << mcu::LeanStreamIO::endl;
  this->leftMotor.pwmOverride(abs(leftSpeed));
  this->rightMotor.pwmOverride(abs(rightSpeed));
}

void Buggy::handlePacket(Packet packet) {
  switch (packet.type) {
    case PacketType::COMMAND:
      mcu::logger << "RECIEVED COMMAND PACKET" << mcu::LeanStreamIO::endl;
      this->handleCommand(packet.content.commandPacket);
      break;
    case PacketType::CONTROL:
      mcu::logger << "RECEIVED CONTROL PACKET" << mcu::LeanStreamIO::endl;
      this->handleControlPacket(packet.content.controlPacket);
  }
}

void Buggy::sendStatusPacket() {
  this->server.sendPacket(
    PacketFactory::createStatusPacket(
      this->ultrasonicSensor.objectDetected(),
      this->ultrasonicSensor.getReading(),
      this->leftMotor.getSpeed(),
      this->rightMotor.getSpeed(),
      this->leftMotor.getDistanceTraveled(),
      this->rightMotor.getDistanceTraveled(),
      this->leftIrSensor.getManualReading(),
      this->rightIrSensor.getManualReading()
    )
  );
}

void Buggy::update(double dt) {
  Packet packet = this->server.update();
  if (packet.type) this->handlePacket(packet);
  this->sendStatusPacket();
  this->currentState->update(*this, dt);
}

void Buggy::setState(BuggyState& newState) {
  this->currentState->exit(*this);
  this->currentState = &newState;
  this->currentState->enter(*this);
}