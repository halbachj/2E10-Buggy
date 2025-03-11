#include "Buggy.hpp"

using EmbeddedLogger::logger;
using logLevel = EmbeddedLogger::LogLevel;

Buggy::Buggy(MotorDriver& leftMotor, MotorDriver& rightMotor, IrSensor& leftIrSensor, IrSensor& rightIrSensor,
   UltrasonicSensor& ultrasonicSensor, BuggyWiFi& wifi, TcpServer& server, LineFollower& lineFollower):
   leftMotor(leftMotor), rightMotor(rightMotor), leftIrSensor(leftIrSensor), rightIrSensor(rightIrSensor), ultrasonicSensor(ultrasonicSensor),
   wifi(wifi), server(server), lineFollower(lineFollower), currentState(&IdleState::instance()) {
  
}

void Buggy::handleCommand(CommandPacket command) {
    logger << String(command.type).c_str() << EmbeddedLogger::endl;
    switch (command.type) {
        case CommandType::START:
            logger << logLevel::DEBUG << "START COMMAND" << EmbeddedLogger::endl;
            this->setState(LineFollowingState::instance());
            break;
        case CommandType::STOP:
            this->setState(IdleState::instance());
            logger << logLevel::DEBUG << "STOP COMMAND" << EmbeddedLogger::endl;
            break;
        case CommandType::RESET_DISTANCE: // New case for resetting distance
            leftMotor.resetDistance();
            rightMotor.resetDistance();
            logger << logLevel::DEBUG << "DISTANCE RESET" << EmbeddedLogger::endl;
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

  logger << String(leftSpeed).c_str() << EmbeddedLogger::endl;
  logger << String(rightSpeed).c_str() << EmbeddedLogger::endl;
  this->leftMotor.pwmOverride(abs(leftSpeed));
  this->rightMotor.pwmOverride(abs(rightSpeed));
}

void Buggy::handlePacket(Packet packet) {
  switch (packet.type) {
    case PacketType::COMMAND:
      logger << logLevel::DEBUG << "RECIEVED COMMAND PACKET" << EmbeddedLogger::endl;
      this->handleCommand(packet.content.commandPacket);
      break;
    case PacketType::CONTROL:
      logger << logLevel::DEBUG << "RECEIVED CONTROL PACKET" << EmbeddedLogger::endl;
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
  this->currentState->exit(*this, &newState);
  BuggyState* oldState = this->currentState;
  this->currentState = &newState;
  this->currentState->enter(*this, oldState);
}