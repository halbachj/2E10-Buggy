#include "RoadSigns.hpp"
#include "EmbeddedLogger.hpp"

using EmbeddedLogger::logger;
using logLevel = EmbeddedLogger::LogLevel;

const RoadSign knownSigns[] = {
  RoadSign{1, "SIGN 1", &JustDriveState::instance()},
  RoadSign{2, "SIGN 2", &JustDriveState::instance()},
};

RoadSignRecognition::RoadSignRecognition(Camera& camera) : camera(camera) {}


void RoadSignRecognition::updateObservedSigns() {
  HUSKYLENSResult block = camera.getBiggestBlock(); //TODO: PASS HUSKYLENSResult AS AN OUTPUT PARAMETER VIA REFERENCE USE A BOOLEAN RETURN TO VERIFY THAT THERE ARE BLOCKS IN THE FRAME
  logger << logLevel::DEBUG << "BIGGEST BLOCK: " << block.ID << EmbeddedLogger::endl;
  int signIndex = block.ID - 1;
  logger << logLevel::DEBUG << "Sign index: " << signIndex << EmbeddedLogger::endl;
  if (signIndex < 0) {
    logger << logLevel::WARNING << "Sign not recognized" << EmbeddedLogger::endl;
    return;
  }
  
  RoadSign sign = knownSigns[signIndex];
  int sign_size = block.width * block.height;
  //IdentifiedRoadSign *observedSign = new IdentifiedRoadSign{sign, millis(), sign_size, 1.0/sign_size};
  // This distance approximation is not realistic, but I didn't find anything about the lens to calculate this for now 
  logger << logLevel::DEBUG << "Observed sign: " << sign.name << EmbeddedLogger::endl;
  /*
  // insert new sign based on size. move the other signs back and remove the leftover
  IdentifiedRoadSign *temp;
  for (int i=0;i<this->maxObservedSigns;i++) {
    if (this->observed_signs[i] == nullptr) { // sign buffer empty at i
      this->observed_signs[i] = observedSign;
      temp = nullptr;
      break; // we have inserted observed sign without the need to move other signs.
    } else if (this->observed_signs[i]->sign.id == observedSign->sign.id) {   // This is the same sign id but it doesn't necessarily mean that is is comes from the same sign. We assume it is for not
      delete this->observed_signs[i];
      this->observed_signs[i] = observedSign;
      temp = nullptr;
      break; // we have inserted observed sign without the need to move other signs.
    } else if (this->observed_signs[i]->size < observedSign->size) { // the observed sign is bigger than the compared one
      temp = this->observed_signs[i];
      this->observed_signs[i] = observedSign;
      observedSign = temp;
    }
  }

  if (temp != nullptr) delete temp; // delete the sign that has become irellevant

  */
}


void RoadSignRecognition::update() {
  updateObservedSigns();

  if (this->observed_signs[0] != nullptr && this->observed_signs[0]->timestamp + 0.75e-3 >= millis()) { // the biggest sign has been there for at least 0.75 seconds
    this->nextRoadSign = this->observed_signs[0];
    logger << logLevel::DEBUG << "Sign observed for at least 750ms" << EmbeddedLogger::endl;
  }



}