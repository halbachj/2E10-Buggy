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
  buggy->setState(LineFollowingState_TURN_LEFT::instance());
}

void sign4Callback(RoadSignRecognition& signRecognition, Buggy* buggy) {
  logger << logLevel::DEBUG << "Sign 4 callback" << EmbeddedLogger::endl;
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
  IdentifiedRoadSign *observedSign = new IdentifiedRoadSign{sign, millis(), sign_size, 1.0/sign_size};
  logger << logLevel::DEBUG << "Observed sign: " << sign.name << EmbeddedLogger::endl;

  IdentifiedRoadSign *temp;
  for (int i=0;i<this->maxObservedSigns;i++) {
    if (this->observed_signs[i] == nullptr) { // sign buffer empty at i
      this->observed_signs[i] = observedSign;
      temp = nullptr;
      break; // we have inserted observed sign without the need to move other signs.
    } else if (this->observed_signs[i]->sign.id == observedSign->sign.id) {   // This is the same sign id but it doesn't necessarily mean that is is comes from the same sign. We assume it is for now
      this->observed_signs[i]->size = observedSign->size;
      this->observed_signs[i]->estDistance = observedSign->estDistance;
      temp = observedSign;
      break; // we have inserted observed sign without the need to move other signs.
    } else if (millis() - this->observed_signs[i]->timestamp > 1000) { // old sign should remove
      temp = this->observed_signs[i];
      this->observed_signs[i] = observedSign;
      break; // inserted withouth moving

    } else if (this->observed_signs[i]->size < observedSign->size) { // the observed sign is bigger than the compared one
      temp = this->observed_signs[i];
      this->observed_signs[i] = observedSign;
      observedSign = temp;
    }
  }

  if (temp != nullptr) delete temp; // delete the sign that has become irellevant

}

void RoadSignRecognition::clearSign() {
  //delete this->nextRoadSign;
  this->nextRoadSign = nullptr;
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
  if (this->nextRoadSign) return; // We have a next road sign. Lets wait until we passed it to get the next one.
  if (this->observed_signs[0] != nullptr) {
    logger << logLevel::DEBUG << "TOP SIGN OBSERVED SINCE: " << this->observed_signs[0]->timestamp << EmbeddedLogger::endl; 
    if (this->observed_signs[0]->timestamp) {
      this->nextRoadSign = this->observed_signs[0];
      logger << logLevel::DEBUG << "Sign observed for at least 750ms" << EmbeddedLogger::endl;
      this->nextRoadSign->sign.callback(*this, this->buggy); 
    }
  }
}