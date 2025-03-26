#include "Camera.hpp"
#include "EmbeddedLogger.hpp"


using EmbeddedLogger::logger;
using logLevel = EmbeddedLogger::LogLevel;


Camera::Camera(HUSKYLENS& huskylens) : huskylens(huskylens) {
}


void Camera::begin() {
  while (!huskylens.begin(Wire)) {
    logger << logLevel::WARNING << F("Begin failed!") << EmbeddedLogger::endl;
    logger << logLevel::WARNING << F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)") << EmbeddedLogger::endl;
    logger << logLevel::WARNING << F("2.Please recheck the connection.") << EmbeddedLogger::endl;
    delay(100);
  }
}


void Camera::update() {
  if (!huskylens.request()) {
    logger << logLevel::WARNING << F("Fail to request data from HUSKYLENS, recheck the connection!") << EmbeddedLogger::endl;
    return;
  }
}    
  
void Camera::switchMode(CameraMode mode) {
  switch (mode) {
    case CameraMode::FACE_RECOGNITION:
      this->huskylens.writeAlgorithm(ALGORITHM_FACE_RECOGNITION); break;
    case CameraMode::OBJECT_TRACKING:
      this->huskylens.writeAlgorithm(ALGORITHM_OBJECT_TRACKING); break;
    case CameraMode::OBJECT_RECOGNITION:
      this->huskylens.writeAlgorithm(ALGORITHM_OBJECT_RECOGNITION); break;
    case CameraMode::LINE_TRACKING:
      this->huskylens.writeAlgorithm(ALGORITHM_LINE_TRACKING); break;
    case CameraMode::COLOR_RECOGNITION:
      this->huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION); break;
    case CameraMode::TAG_RECOGNITION:
      this->huskylens.writeAlgorithm(ALGORITHM_TAG_RECOGNITION); break;
  }
}


HUSKYLENSResult Camera::getBiggestBlock() {
  int count, best_size, tmp_size;
  HUSKYLENSResult result, tmp;

  this->update();
  count = this->huskylens.countBlocksLearned();
  if (count < 1) return HUSKYLENSResult{};

  result = this->huskylens.getBlockLearned(0);
  best_size = result.width * result.height;
  for (int i=1;i<count;++i) {
    tmp = this->huskylens.getBlockLearned(i);
    tmp_size = tmp.width * tmp.height;
    if (tmp_size > best_size) result = tmp;
  }
  return result;
}

HUSKYLENSResult Camera::getBiggestArrow() {
  int count, best_size;
  HUSKYLENSResult result, tmp;

  this->update();
  count = this->huskylens.countArrows();
  if (count < 1) return HUSKYLENSResult{};

  result = this->huskylens.getArrow(0);
  /*best_size = sqrt( (  ) );
  for (int i=1;i<count;++i) {
    tmp = getBlockLearned(i);
    size = tmp.width * tmp.height;
    if (size > best_size) result = tmp;
  }*/
  return result;
}

RoadSign Camera::readRoadSign() {
  
}