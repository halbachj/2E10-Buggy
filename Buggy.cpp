#include "Buggy.hpp"

Buggy::Buggy(MotorDriver& leftMotor, MotorDriver& rightMotor, IrSensor& leftIrSensor, IrSensor& rightIrSensor,
   UltrasonicSensor& ultrasonicSensor, BuggyWiFi& wifi, TcpServer& server, LineFollower& lineFollower):
   leftMotor(leftMotor), rightMotor(rightMotor), leftIrSensor(leftIrSensor), rightIrSensor(rightIrSensor), ultrasonicSensor(ultrasonicSensor),
   wifi(wifi), server(server), lineFollower(lineFollower), currentState(&IdleState::instance()) {
  
}

void Buggy::handleCommand(CommandPacket command) {
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

void Buggy::handlePacket(Packet packet) {
  switch (packet.type) {
    case PacketType::COMMAND:
      mcu::logger << "RECIEVED COMMAND PACKET" << mcu::LeanStreamIO::endl;
      this->handleCommand(packet.content.commandPacket);
      break;
    default:
      break;
  }
}

void Buggy::sendStatusPacket() {
  this->server.sendPacket(
    PacketFactory::createStatusPacket(
      this->objectDetected,
      this->ultrasonicSensor.getReading(),
      this->leftMotor.getSpeed(),
      this->rightMotor.getSpeed()
    )
  );
}

void Buggy::update(unsigned int dt) {
  Packet packet = this->server.update();
  if (!packet.type) this->handlePacket(packet);
  this->currentState->update(*this, dt);
  //this->sendStatusPacket();
}

void Buggy::setState(BuggyState& newState) {
  this->currentState->exit(*this);
  this->currentState = &newState;
  this->currentState->enter(*this);
}