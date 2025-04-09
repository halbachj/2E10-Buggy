#include "RoadSigns.hpp"
#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;
using logLevel = EmbeddedLogger::LogLevel;

void sign1Callback(RoadSignRecognition& signRecognition, Buggy* buggy) {
  logger << logLevel::DEBUG << "Sign 1 callback" << EmbeddedLogger::endl;
  buggy->setSpeed(200);
  signRecognition.clearSign();
}


void sign2Callback(RoadSignRecognition& signRecognition, Buggy* buggy) {
  logger << logLevel::DEBUG << "Sign 2 callback" << EmbeddedLogger::endl;
  buggy->setSpeed(400);
  signRecognition.clearSign();
}

void sign3Callback(RoadSignRecognition& signRecognition, Buggy* buggy) {
  logger << logLevel::DEBUG << "Sign 3 callback" << EmbeddedLogger::endl;
  buggy->setSpeed(100);
  buggy->setState(LineFollowingState_TURN_LEFT::instance());
}

void sign4Callback(RoadSignRecognition& signRecognition, Buggy* buggy) {
  logger << logLevel::DEBUG << "Sign 4 callback" << EmbeddedLogger::endl;
  buggy->setSpeed(100);
  buggy->setState(LineFollowingState_TURN_RIGHT::instance());
}

const RoadSign knownSigns[] = {
  RoadSign{1, "SIGN 1", &sign1Callback},
  RoadSign{2, "SIGN 2", &sign2Callback},
  RoadSign{3, "SIGN 3", &sign3Callback},
  RoadSign{4, "SIGN 4", &sign4Callback},
};

RoadSignRecognition::RoadSignRecognition(Camera& camera) : camera(camera) {}


void RoadSignRecognition::updateObservedSigns() {
  HUSKYLENSResult block;
  if(!camera.getBiggestBlock(&block)) {
    logger << logLevel::WARNING << "NO SIGN IN FRAME" << EmbeddedLogger::endl;
  }
  logger << logLevel::DEBUG << "BIGGEST BLOCK: " << block.ID << EmbeddedLogger::endl;
  int signIndex = block.ID - 1;
  logger << logLevel::DEBUG << "Sign index: " << signIndex << EmbeddedLogger::endl;
  if (signIndex < 0) {
    logger << logLevel::WARNING << "Sign not recognized" << EmbeddedLogger::endl;
    return;
  }
  
  RoadSign sign = knownSigns[signIndex];
  int sign_size = block.width * block.height;
  if (sign.id == this->lastRoadSign.sign.id) return;

  IdentifiedRoadSign observedSign = IdentifiedRoadSign{sign, millis(), sign_size, 1.0/sign_size};
  logger << logLevel::DEBUG << "Observed sign: " << sign.name << EmbeddedLogger::endl;
  this->nextRoadSign = observedSign;
  this->lastRoadSign = this->nextRoadSign;
  this->readSign = true;
}

void RoadSignRecognition::clearSign() {
  //this->nextRoadSign = IdentifiedRoadSign{};
  this->readSign = false;
}


void RoadSignRecognition::begin(Buggy* buggy) {
  this->buggy = buggy;
  this->initialized = true;
}

void RoadSignRecognition::update() {
  if (!this->initialized) {
    logger << logLevel::WARNING << "SIGN RECOGNITION NOT INITIALIZED" << EmbeddedLogger::endl;
    return;
  }

  updateObservedSigns();

  if (!this->readSign) return; // We have a next road sign. Lets wait until we passed it to get the next one.
  logger << logLevel::DEBUG << "TOP SIGN OBSERVED SINCE: " << this->nextRoadSign.timestamp << EmbeddedLogger::endl; 
  //if (millis() - this->nextRoadSign.timestamp >= 10e-3) {
  //  logger << logLevel::DEBUG << "Sign observed for at least 10ms" << EmbeddedLogger::endl;
  this->nextRoadSign.sign.callback(*this, this->buggy);
  //}

  //if(millis() - this->nextRoadSign.timestamp >= 6000) this->clearSign();
}